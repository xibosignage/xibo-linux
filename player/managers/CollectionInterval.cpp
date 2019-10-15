#include "CollectionInterval.hpp"

#include "common/dt/DateTime.hpp"
#include "common/dt/Timer.hpp"
#include "common/logger/Logging.hpp"
#include "common/logger/XmlLogsRetriever.hpp"
#include "config.hpp"

#include "managers/StatsRecorder.hpp"
#include "networking/xmds/XmdsRequestSender.hpp"

#include <glibmm/main.h>

const uint DefaultInterval = 900;
namespace ph = std::placeholders;

CollectionInterval::CollectionInterval(XmdsRequestSender& xmdsSender) :
    xmdsSender_{xmdsSender},
    intervalTimer_{std::make_unique<Timer>()},
    collectInterval_{DefaultInterval},
    started_{false},
    registered_{false},
    requiredFiles_{0}
{
    assert(intervalTimer_);
}

void CollectionInterval::startRegularCollection()
{
    started_ = true;
    collect(std::bind(&CollectionInterval::onRegularCollectionFinished, this, ph::_1));
}

void CollectionInterval::stop()
{
    workerThread_.reset();
}

void CollectionInterval::startTimer()
{
    intervalTimer_->start(std::chrono::seconds(collectInterval_), [=]() {
        collect(std::bind(&CollectionInterval::onRegularCollectionFinished, this, ph::_1));
    });
}

void CollectionInterval::onRegularCollectionFinished(const PlayerError& error)
{
    collectionFinished_.emit(error);
    startTimer();

    Log::debug("[CollectionInterval] Finished. Next collection will start in {} seconds", collectInterval_);
}

void CollectionInterval::collect(CollectionResultCallback callback)
{
    workerThread_ = std::make_unique<JoinableThread>([=]() {
        Log::debug("[CollectionInterval] Started");

        auto session = std::make_shared<CollectionSession>();
        session->callback = callback;

        auto registerDisplayResult =
            xmdsSender_.registerDisplay(ProjectResources::codeVersion(), ProjectResources::version(), "Display").get();
        onDisplayRegistered(registerDisplayResult, session);
    });
}

void CollectionInterval::sessionFinished(CollectionSessionPtr session, PlayerError error)
{
    Glib::MainContext::get_default()->invoke([this, session, error]() {
        if (started_)
        {
            startTimer();
        }
        session->callback(error);
        return false;
    });
}

void CollectionInterval::onDisplayRegistered(const ResponseResult<RegisterDisplay::Result>& registerDisplay,
                                             CollectionSessionPtr session)
{
    auto [error, result] = registerDisplay;
    if (!error)
    {
        auto displayError = getDisplayStatus(result.status);
        if (!displayError)
        {
            Log::debug("[XMDS::RegisterDisplay] Success");

            registered_ = true;
            lastChecked_ = DateTime::now();
            settingsUpdated_.emit(result.playerSettings);

            auto requiredFilesResult = xmdsSender_.requiredFiles().get();
            auto scheduleResult = xmdsSender_.schedule().get();

            onSchedule(scheduleResult, session);
            onRequiredFiles(requiredFilesResult, session);

            XmlLogsRetriever logsRetriever;
            auto submitLogsResult = xmdsSender_.submitLogs(logsRetriever.retrieveLogs()).get();
            onSubmitLog(submitLogsResult, session);

            auto submitStatsResult = xmdsSender_.submitStats(StatsRecorder::get().xmlString()).get();
            onSubmitStats(submitStatsResult, session);
        }
        sessionFinished(session, displayError);
    }
    else
    {
        sessionFinished(session, error);
    }
}

PlayerError CollectionInterval::getDisplayStatus(const RegisterDisplay::Result::Status& status)
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

void CollectionInterval::statsAggregation(const std::string& aggregationLevel)
{
    statsAggregation_ = aggregationLevel;
}

CmsStatus CollectionInterval::status()
{
    CmsStatus status;

    status.registered = registered_;
    status.lastChecked = lastChecked_;
    status.requiredFiles = requiredFiles_;

    return status;
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

void CollectionInterval::onRequiredFiles(const ResponseResult<RequiredFiles::Result>& requiredFiles,
                                         CollectionSessionPtr session)
{
    auto [error, result] = requiredFiles;
    if (!error)
    {
        Log::debug("[XMDS::RequiredFiles] Received");

        RequiredFilesDownloader downloader{xmdsSender_};

        auto&& files = result.requiredFiles();
        auto&& resources = result.requiredResources();

        requiredFiles_ = files.size() + resources.size();

        auto resourcesResult = downloader.download(resources);
        auto filesResult = downloader.download(files);

        updateMediaInventory(filesResult.get());
        updateMediaInventory(resourcesResult.get());

        filesDownloaded_.emit();
    }
    else
    {
        sessionFinished(session, error);
    }
}

void CollectionInterval::onSchedule(const ResponseResult<Schedule::Result>& schedule, CollectionSessionPtr session)
{
    auto [error, result] = schedule;
    if (!error)
    {
        Log::debug("[XMDS::Schedule] Received");
        scheduleAvailable_.emit(result);
    }
    else
    {
        sessionFinished(session, error);
    }
}

void CollectionInterval::updateMediaInventory(MediaInventoryItems&& items)
{
    xmdsSender_.mediaInventory(std::move(items)).then([](auto future) {
        auto [error, result] = future.get();
        if (error)
        {
            Log::error("[XMDS::MediaInventory] {}", error);
        }
        else
        {
            std::string message = result.success ? "Updated" : "Not updated";
            Log::debug("[XMDS::MediaInventory] {}", message);
        }
    });
}

void CollectionInterval::onSubmitLog(const ResponseResult<SubmitLog::Result>& logResult, CollectionSessionPtr session)
{
    auto [error, result] = logResult;
    if (!error)
    {
        if (result.success)
        {
            Log::debug("[XMDS::SubmitLogs] Submitted");
        }
        else
        {
            Log::error("[XMDS::SubmitLogs] Not submited due to unknown error");
        }
    }
    else
    {
        sessionFinished(session, error);
    }
}

void CollectionInterval::onSubmitStats(const ResponseResult<SubmitStats::Result>& statsResult,
                                       CollectionSessionPtr session)
{
    auto [error, result] = statsResult;
    if (!error)
    {
        if (result.success)
        {
            Log::debug("[XMDS::SubmitStats] Submitted");
        }
        else
        {
            Log::error("[XMDS::SubmitStats] Not submited due to unknown error");
        }
    }
    else
    {
        sessionFinished(session, error);
    }
}
