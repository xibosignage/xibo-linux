#include "CollectionInterval.hpp"

#include "MainLoop.hpp"

#include "common/dt/DateTime.hpp"
#include "common/dt/Timer.hpp"
#include "common/logger/Logging.hpp"
#include "common/logger/XmlLogsRetriever.hpp"
#include "config/AppConfig.hpp"

#include "cms/xmds/XmdsRequestSender.hpp"
#include "stat/StatsFormatter.hpp"
#include "stat/StatsRecorder.hpp"

namespace ph = std::placeholders;

CollectionInterval::CollectionInterval(XmdsRequestSender& xmdsSender,
                                       StatsRecorder& statsRecorder,
                                       FileCache& fileCache) :
    xmdsSender_{xmdsSender},
    statsRecorder_{statsRecorder},
    fileCache_{fileCache},
    intervalTimer_{std::make_unique<Timer>()},
    collectInterval_{DefaultInterval},
    running_{false},
    status_{}
{
    assert(intervalTimer_);
}

bool CollectionInterval::running() const
{
    return running_;
}

void CollectionInterval::stop()
{
    workerThread_.reset();
}

void CollectionInterval::startTimer()
{
    intervalTimer_->startOnce(std::chrono::seconds(collectInterval_), [this]() { collectNow(); });
}

void CollectionInterval::collectNow()
{
    if (!running_)
    {
        running_ = true;
        workerThread_ = std::make_unique<JoinableThread>([=]() {
            Log::debug("[CollectionInterval] Started");

            auto registerDisplayResult =
                xmdsSender_.registerDisplay(AppConfig::codeVersion(), AppConfig::version(), "Display").get();
            onDisplayRegistered(registerDisplayResult);
        });
    }
}

void CollectionInterval::sessionFinished(const PlayerError& error)
{
    running_ = false;
    startTimer();
    Log::debug("[CollectionInterval] Finished. Next collection will start in {} seconds", collectInterval_);

    MainLoop::pushToUiThread([this, error]() { collectionFinished_(error); });
}

void CollectionInterval::onDisplayRegistered(const ResponseResult<RegisterDisplay::Result>& registerDisplay)
{
    auto [error, result] = registerDisplay;
    if (!error)
    {
        auto displayError = displayStatus(result.status);
        if (!displayError)
        {
            Log::debug("[XMDS::RegisterDisplay] Success");

            status_.registered = true;
            status_.lastChecked = DateTime::now();

            MainLoop::pushToUiThread([this, result = std::move(result.playerSettings)]() { settingsUpdated_(result); });

            auto requiredFilesResult = xmdsSender_.requiredFiles().get();
            auto scheduleResult = xmdsSender_.schedule().get();

            onSchedule(scheduleResult);
            onRequiredFiles(requiredFilesResult);

            XmlLogsRetriever logsRetriever;
            auto submitLogsResult = xmdsSender_.submitLogs(logsRetriever.retrieveLogs()).get();
            onSubmitted("SubmitLogs", submitLogsResult);

            if (!statsRecorder_.empty())
            {
                StatsFormatter formatter;
                auto submitStatsResult = xmdsSender_.submitStats(formatter.toXml(statsRecorder_.records())).get();
                statsRecorder_.clear();
                onSubmitted("SubmitStats", submitStatsResult);
            }
        }
        sessionFinished(displayError);
    }
    else
    {
        sessionFinished(error);
    }
}

PlayerError CollectionInterval::displayStatus(const RegisterDisplay::Result::Status& status)
{
    using DisplayCode = RegisterDisplay::Result::Status::Code;

    switch (status.code)
    {
        case DisplayCode::Ready: return {};
        case DisplayCode::Added:
        case DisplayCode::Waiting: return {"CMS", status.message};
        default: return {"CMS", "Unknown error with RegisterDisplay"};
    }
}

void CollectionInterval::updateInterval(int collectInterval)
{
    if (collectInterval_ != collectInterval)
    {
        Log::debug("[CollectionInterval] Interval updated to {} seconds", collectInterval);
        collectInterval_ = collectInterval;
    }
}

// TODO potential data race here
CmsStatus CollectionInterval::status() const
{
    return status_;
}

SignalSettingsUpdated& CollectionInterval::settingsUpdated()
{
    return settingsUpdated_;
}

SignalScheduleAvailable& CollectionInterval::scheduleAvailable()
{
    return scheduleAvailable_;
}

SignalCollectionFinished& CollectionInterval::collectionFinished()
{
    return collectionFinished_;
}

SignalFilesDownloaded& CollectionInterval::filesDownloaded()
{
    return filesDownloaded_;
}

void CollectionInterval::onRequiredFiles(const ResponseResult<RequiredFiles::Result>& requiredFiles)
{
    auto [error, result] = requiredFiles;
    if (!error)
    {
        Log::debug("[XMDS::RequiredFiles] Received");

        RequiredFilesDownloader downloader{xmdsSender_, fileCache_};

        auto&& files = result.requiredFiles();
        auto&& resources = result.requiredResources();

        status_.requiredFiles = files.size() + resources.size();

        auto resourcesResult = downloader.download(resources);
        auto filesResult = downloader.download(files);

        updateMediaInventory(filesResult.get());
        updateMediaInventory(resourcesResult.get());

        MainLoop::pushToUiThread([this]() { filesDownloaded_(); });
    }
    else
    {
        sessionFinished(error);
    }
}

void CollectionInterval::onSchedule(const ResponseResult<Schedule::Result>& schedule)
{
    auto [error, result] = schedule;
    if (!error)
    {
        Log::debug("[XMDS::Schedule] Received");
        MainLoop::pushToUiThread([this, result = std::move(result)]() {
            scheduleAvailable_(LayoutSchedule::fromString(result.scheduleXml));
        });
    }
    else
    {
        sessionFinished(error);
    }
}

void CollectionInterval::updateMediaInventory(MediaInventoryItems&& items)
{
    onSubmitted("MediaInventory", xmdsSender_.mediaInventory(std::move(items)).get());
}

template <typename Result>
void CollectionInterval::onSubmitted(std::string_view requestName, const ResponseResult<Result>& submitResult)
{
    auto [error, result] = submitResult;
    if (!error)
    {
        if (result.success)
        {
            Log::debug("[XMDS::{}] Submitted", requestName);
        }
        else
        {
            Log::error("[XMDS::{}] Not submited due to unknown error", requestName);
        }
    }
    else
    {
        sessionFinished(error);
    }
}
