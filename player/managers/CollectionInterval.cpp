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
    m_xmdsSender{xmdsSender},
    m_intervalTimer{std::make_unique<Timer>()},
    m_collectInterval{DefaultInterval}
{
}

void CollectionInterval::startRegularCollection()
{
    m_started = true;
    collect(std::bind(&CollectionInterval::onRegularCollectionFinished, this, ph::_1));
}

void CollectionInterval::stop()
{
    m_workerThread.reset();
}

void CollectionInterval::startTimer()
{
    m_intervalTimer->start(std::chrono::seconds(m_collectInterval), [=]() {
        collect(std::bind(&CollectionInterval::onRegularCollectionFinished, this, ph::_1));
    });
}

void CollectionInterval::onRegularCollectionFinished(const PlayerError& error)
{
    m_collectionFinished.emit(error);
    startTimer();

    Log::debug("[CollectionInterval] Finished. Next collection will start in {} seconds", m_collectInterval);
}

void CollectionInterval::collect(CollectionResultCallback callback)
{
    m_workerThread = std::make_unique<JoinableThread>([=]() {
        Log::debug("[CollectionInterval] Started");

        auto session = std::make_shared<CollectionSession>();
        session->callback = callback;

        auto registerDisplayResult =
            m_xmdsSender.registerDisplay(ProjectResources::codeVersion(), ProjectResources::version(), "Display").get();
        onDisplayRegistered(registerDisplayResult, session);
    });
}

void CollectionInterval::sessionFinished(CollectionSessionPtr session, PlayerError error)
{
    Glib::MainContext::get_default()->invoke([this, session, error]() {
        if (m_started)
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

            m_registered = true;
            m_lastChecked = DateTime::now();
            m_settingsUpdated.emit(result.playerSettings);

            auto requiredFilesResult = m_xmdsSender.requiredFiles().get();
            auto scheduleResult = m_xmdsSender.schedule().get();

            onSchedule(scheduleResult, session);
            onRequiredFiles(requiredFilesResult, session);

            submitScreenShot();

            XmlLogsRetriever logsRetriever;
            auto submitLogsResult = m_xmdsSender.submitLogs(logsRetriever.retrieveLogs()).get();
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
    if (m_collectInterval != collectInterval)
    {
        Log::debug("[CollectionInterval] Interval updated. Old: {}, New: {}", m_collectInterval, collectInterval);
        m_collectInterval = collectInterval;
    }
}

CmsStatus CollectionInterval::status()
{
    CmsStatus status;

    status.registered = m_registered;
    status.lastChecked = m_lastChecked;
    status.requiredFiles = m_requiredFiles;

    return status;
}

SignalSettingsUpdated& CollectionInterval::settingsUpdated()
{
    return m_settingsUpdated;
}

SignalScheduleAvailable& CollectionInterval::scheduleAvailable()
{
    return m_scheduleAvailable;
}

SignalCollectionFinished& CollectionInterval::collectionFinished()
{
    return m_collectionFinished;
}

SignalFilesDownloaded& CollectionInterval::filesDownloaded()
{
    return m_filesDownloaded;
}

void CollectionInterval::onRequiredFiles(const ResponseResult<RequiredFiles::Result>& requiredFiles,
                                         CollectionSessionPtr session)
{
    auto [error, result] = requiredFiles;
    if (!error)
    {
        Log::debug("[RequiredFiles] Received");

        RequiredFilesDownloader downloader{m_xmdsSender};

        auto&& files = result.requiredFiles();
        auto&& resources = result.requiredResources();

        m_requiredFiles = files.size() + resources.size();

        auto resourcesResult = downloader.download(resources);
        auto filesResult = downloader.download(files);

        updateMediaInventory(filesResult.get());
        updateMediaInventory(resourcesResult.get());

        m_filesDownloaded.emit();
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
        m_scheduleAvailable.emit(result);
    }
    else
    {
        sessionFinished(session, error);
    }
}

void CollectionInterval::updateMediaInventory(MediaInventoryItems&& items)
{
    m_xmdsSender.mediaInventory(std::move(items)).then([](auto future) {
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
        m_xmdsSender.submitScreenShot(screenshot).then([](auto future) {
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
