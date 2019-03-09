#include "CollectionInterval.hpp"

#include "xmds/XMDSManager.hpp"
#include "events/CallbackGlobalQueue.hpp"

#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "utils/TimerProvider.hpp"

#include <glibmm/main.h>

const uint DEFAULT_INTERVAL = 900;
namespace ph = std::placeholders;

CollectionInterval::CollectionInterval() :
    m_intervalTimer{std::make_unique<TimerProvider>()}, m_collectInterval{DEFAULT_INTERVAL}
{
}

void CollectionInterval::startRegularCollection()
{
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
    pushEvent(CollectionFinishedEvent{error});
    startTimer();
}

void CollectionInterval::collectOnce(CollectionResultCallback callback)
{
    m_workerThread = std::make_unique<JoinableThread>([=]()
    {
        Log::debug("Collection started");

        auto session = std::make_shared<CollectionSession>();
        session->callback = callback;

        auto registerDisplayResult = Utils::xmdsManager().registerDisplay(121, "1.8", "Display").get();
        onDisplayRegistered(registerDisplayResult, session);
    });
}

void CollectionInterval::sessionFinished(CollectionSessionPtr session, PlayerError error)
{
    callbackQueue().add([session, error](){
        session->callback(error);
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
            pushEvent(SettingsUpdatedEvent{result.playerSettings});

            auto requiredFilesResult = Utils::xmdsManager().requiredFiles().get();
            auto scheduleResult = Utils::xmdsManager().schedule().get();

            onSchedule(scheduleResult, session);
            onRequiredFiles(requiredFilesResult, session);
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

void CollectionInterval::onRequiredFiles(const ResponseResult<RequiredFiles::Result>& requiredFiles, CollectionSessionPtr session)
{
    auto [error, result] = requiredFiles;
    if(!error)
    {
        RequiredFilesDownloader downloader;

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
        pushEvent(ScheduleUpdatedEvent{result.schedule});
    }
    else
    {
        sessionFinished(session, error);
    }
}

void CollectionInterval::updateMediaInventory(MediaInventoryItems&& items)
{
    Utils::xmdsManager().mediaInventory(std::move(items)).then([](boost::future<ResponseResult<MediaInventory::Result>> future){
        auto [error, result] = future.get();
        if(error)
        {
            Log::error("MediaInventory: {}", error);
        }
    });
}
