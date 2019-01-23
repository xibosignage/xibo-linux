#include "CollectionInterval.hpp"

#include "xmds/XMDSManager.hpp"
#include "events/CallbackGlobalQueue.hpp"

#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "utils/TimerProvider.hpp"

#include <glibmm/main.h>
#include <future>

const uint DEFAULT_INTERVAL = 5;
namespace ph = std::placeholders;

CollectionInterval::CollectionInterval() :
    m_collectInterval{DEFAULT_INTERVAL},  m_intervalTimer(std::make_unique<TimerProvider>())
{
}

void CollectionInterval::startRegularCollection()
{
    startTimer();
}

void CollectionInterval::startTimer()
{
    m_intervalTimer->startOnceSeconds(static_cast<unsigned int>(m_collectInterval), [=](){
        collectOnce(std::bind(&CollectionInterval::onRegularCollectionFinished, this, ph::_1));
    });
}

void CollectionInterval::onRegularCollectionFinished(const CollectionResult& result)
{
    Log::debug("Collection finished {}", std::this_thread::get_id());
    Log::debug("Next collection will start in {} seconds", m_collectInterval);
    pushEvent(CollectionFinished{result});
    startTimer();
}

void CollectionInterval::collectOnce(CollectionResultCallback callback)
{
    m_workerThread = std::make_unique<JoinableThread>([=]()
    {
        Log::debug("Collection started {}", std::this_thread::get_id());

        auto session = std::make_shared<CollectionSession>();
        session->callback = callback;

        auto registerDisplayResult = Utils::xmdsManager().registerDisplay(121, "1.8", "Display").get();
        onDisplayRegistered(registerDisplayResult, session);
    });
}

void CollectionInterval::sessionFinished(CollectionSessionPtr session, boost::system::error_code error)
{
    callbackQueue().add([session, error](){
        session->result.error = error;
        session->callback(session->result);
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
            updateTimer(result.playerSettings.collectInterval);
            session->result.settings = result.playerSettings;

            auto requiredFilesResult = Utils::xmdsManager().requiredFiles().get();
            auto scheduleResult = Utils::xmdsManager().schedule().get();

            onSchedule(scheduleResult, session);
            onRequiredFiles(requiredFilesResult, session);

            sessionFinished(session);
        }
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

void CollectionInterval::updateTimer(int collectInterval)
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
        RequiredFilesDownloader filesDownloader;
        RequiredResourcesDownloader resourcesDownloader;

        auto&& files = result.requiredFiles();
        auto&& resources = result.requiredResources();

        Log::debug("{} files and {} resources to download {}", files.size(), resources.size(), std::this_thread::get_id());

        auto resourcesResult = resourcesDownloader.download(resources);
        auto filesResult = filesDownloader.download(files);

        Log::trace("Waiting for downloads...");
        filesResult.wait();
        Log::debug("Files downloaded");
        resourcesResult.wait();
        Log::debug("Resources downloaded");
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
        session->result.schedule = result;
    }
    else
    {
        sessionFinished(session, error);
    }
}
