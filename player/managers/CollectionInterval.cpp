#include "CollectionInterval.hpp"
#include "config.hpp"

#include "networking/xmds/XmdsRequestSender.hpp"

#include "common/dt/DateTime.hpp"
#include "common/logger/Logging.hpp"

#include "common/dt/Timer.hpp"
#include "utils/Managers.hpp"
#include "utils/ScreenShoter.hpp"
#include "xmlsink/XmlLogsRetriever.hpp"

#include <glibmm/main.h>

const uint DefaultInterval = 900;
namespace ph = std::placeholders;

CollectionInterval::CollectionInterval(XmdsRequestSender& xmdsSender) :
    xmdsSender_{xmdsSender},
    intervalTimer_{std::make_unique<Timer>()},
    collectInterval_{DefaultInterval}
{
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
            Log::debug("[RegisterDisplay] Success");

            registered_ = true;
            lastChecked_ = DateTime::now();
            settingsUpdated_.emit(result.playerSettings);

            auto requiredFilesResult = xmdsSender_.requiredFiles().get();
            auto scheduleResult = xmdsSender_.schedule().get();

            onSchedule(scheduleResult, session);
            onRequiredFiles(requiredFilesResult, session);

            submitScreenShot();

            XmlLogsRetriever logsRetriever;
            auto submitLogsResult = xmdsSender_.submitLogs(logsRetriever.retrieveLogs()).get();
            onSubmitLog(submitLogsResult, session);
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
        case DisplayCode::Waiting: return {PlayerError::Type::CMS, status.message};
        default: return {PlayerError::Type::CMS, "Unknown error with RegisterDisplay"};
    }
}

void CollectionInterval::updateInterval(int collectInterval)
{
    if (collectInterval_ != collectInterval)
    {
        Log::debug("[CollectionInterval] Interval updated. Old: {}, New: {}", collectInterval_, collectInterval);
        collectInterval_ = collectInterval;
    }
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
        Log::debug("[RequiredFiles] Received");

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
        Log::debug("[Schedule] Received");
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
            Log::error("[MediaInventory] {}", error);
        }
        else
        {
            Log::debug("[MediaInventory] Updated");
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
            Log::debug("[SubmitLogs] Submitted");
        }
        else
        {
            Log::debug("[SubmitLogs] Not submited due to unknown error");
        }
    }
    else
    {
        sessionFinished(session, error);
    }
}

void CollectionInterval::submitScreenShot()
{
    Managers::screenShoter().takeBase64([this](const std::string& screenshot) {
        xmdsSender_.submitScreenShot(screenshot).then([](auto future) {
            auto [error, result] = future.get();
            if (error)
            {
                Log::error("[SubmitScreenShot] {}", error);
            }
            else
            {
                Log::debug("[SubmitScreenShot] Submitted");
            }
        });
    });
}
