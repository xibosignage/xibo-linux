#include "CollectionInterval.hpp"

#include "networking/xmds/XmdsRequestSender.hpp"

#include "common/logger/Logging.hpp"
#include "utils/TimerProvider.hpp"
#include "utils/ScreenShoter.hpp"
#include "utils/Managers.hpp"
#include "xmlsink/XmlLogsRetriever.hpp"

#include <glibmm/main.h>

const uint DEFAULT_INTERVAL = 900;
namespace ph = std::placeholders;

CollectionInterval::CollectionInterval(XmdsRequestSender& xmdsSender) :
    m_xmdsSender{xmdsSender}, m_intervalTimer{std::make_unique<TimerProvider>()}, m_collectInterval{DEFAULT_INTERVAL}
{
}

void CollectionInterval::startRegularCollection()
{
    started = true;
    startTimer();
}

void CollectionInterval::stop()
{
    m_workerThread.reset();
}

void CollectionInterval::startTimer()
{
    m_intervalTimer->startOnceSeconds(static_cast<unsigned int>(m_collectInterval), [=](){
        collectOnce(std::bind(&CollectionInterval::onRegularCollectionFinished, this, ph::_1));
    });
}

void CollectionInterval::onRegularCollectionFinished(const PlayerError& error)
{
    Log::debug("Collection finished. Next collection will start in {} seconds", m_collectInterval);
    m_collectionFinished.emit(error);
    startTimer();
}

void CollectionInterval::collectOnce(CollectionResultCallback callback)
{
    m_workerThread = std::make_unique<JoinableThread>([=]()
    {
        Log::debug("Collection started");

        auto session = std::make_shared<CollectionSession>();
        session->callback = callback;

        auto registerDisplayResult = m_xmdsSender.registerDisplay(121, "1.8", "Display").get();
        onDisplayRegistered(registerDisplayResult, session);
    });
}

void CollectionInterval::sessionFinished(CollectionSessionPtr session, PlayerError error)
{
    Glib::MainContext::get_default()->invoke([this, session, error](){
        if(started)
        {
            startTimer();
        }
        session->callback(error);
        return false;
    });
}

void CollectionInterval::onDisplayRegistered(const ResponseResult<RegisterDisplay::Result>& registerDisplay, CollectionSessionPtr session)
{
    auto [error, result] = registerDisplay;
    if(!error)
    {
        displayMessage(result.status);
        if(result.status.code == RegisterDisplay::Result::Status::Code::Ready) // FIXME handle Activated/Waiting
        {
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
        sessionFinished(session);
    }
    else
    {
        sessionFinished(session, error);
    }
}

void CollectionInterval::displayMessage(const RegisterDisplay::Result::Status& status)
{
    if(status.code != RegisterDisplay::Result::Status::Code::Invalid)
    {
        Log::debug(status.message);
    }
}

void CollectionInterval::updateInterval(int collectInterval)
{
    if(m_collectInterval != collectInterval)
    {
        Log::debug("Collection interval updated. Old: {}, New: {}", m_collectInterval, collectInterval);
        m_collectInterval = collectInterval;
    }
}

SignalSettingsUpdated CollectionInterval::settingsUpdated()
{
    return m_settingsUpdated;
}

SignalScheduleUpdated CollectionInterval::scheduleUpdated()
{
    return m_scheduleUpdated;
}

SignalCollectionFinished CollectionInterval::collectionFinished()
{
    return m_collectionFinished;
}

void CollectionInterval::onRequiredFiles(const ResponseResult<RequiredFiles::Result>& requiredFiles, CollectionSessionPtr session)
{
    auto [error, result] = requiredFiles;
    if(!error)
    {
        RequiredFilesDownloader downloader{m_xmdsSender};

        auto&& files = result.requiredFiles();
        auto&& resources = result.requiredResources();

        Log::debug("{} files and {} resources to download", files.size(), resources.size());

        auto resourcesResult = downloader.download(resources);
        auto filesResult = downloader.download(files);

        updateMediaInventory(filesResult.get());
        updateMediaInventory(resourcesResult.get());
    }
    else
    {
        sessionFinished(session, error);
    }

}

void CollectionInterval::onSchedule(const ResponseResult<Schedule::Result>& schedule, CollectionSessionPtr session)
{
    auto [error, result] = schedule;
    if(!error)
    {
        m_scheduleUpdated.emit(result.schedule);
    }
    else
    {
        sessionFinished(session, error);
    }
}

void CollectionInterval::updateMediaInventory(MediaInventoryItems&& items)
{
    m_xmdsSender.mediaInventory(std::move(items)).then([](auto future){
        auto [error, result] = future.get();
        if(error)
        {
            Log::error("MediaInventory: {}", error);
        }
    });
}

void CollectionInterval::onSubmitLog(const ResponseResult<SubmitLog::Result>& logResult, CollectionSessionPtr session)
{
    auto [error, result] = logResult;
    if(!error)
    {
        if(result.success)
        {
            Log::debug("Logs were submitted successfully");
        }
        else
        {
            Log::debug("Logs were not submited due to some error");
        }
    }
    else
    {
        sessionFinished(session, error);
    }
}

void CollectionInterval::submitScreenShot()
{
    Managers::screenShoter().takeBase64([this](const std::string& screenshot){
        m_xmdsSender.submitScreenShot(screenshot).then([](auto future){
            auto [error, result] = future.get();
            if(error)
            {
                Log::error("SubmitScreenShot: {}", error);
            }
        });
    });
}
