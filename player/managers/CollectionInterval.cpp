#include "CollectionInterval.hpp"

#include "xmds/XMDSManager.hpp"

#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "utils/TimerProvider.hpp"

#include <glibmm/main.h>

const uint DEFAULT_INTERVAL = 5;
namespace ph = std::placeholders;

CollectionInterval::CollectionInterval() :
    m_collectInterval{DEFAULT_INTERVAL},  m_intervalTimer(std::make_unique<TimerProvider>())
{
}

void CollectionInterval::start()
{
    startTimer();
}

void CollectionInterval::startTimer()
{
    m_intervalTimer->startOnceSeconds(static_cast<unsigned int>(m_collectInterval), [=](){
        startRegularCollection();
    });
}

void CollectionInterval::onCollectionFinished(const CollectionResult& result)
{
    Log::debug("Collection finished with success result: {}", result.success);
    Log::debug("Next collection will start in {} seconds", m_collectInterval);
    startTimer();
}

void CollectionInterval::collect(CollectionResultCallback callback)
{
    Log::debug("Collection started");

    auto session = std::make_shared<CollectionSession>();
    session->callback = callback;

    auto clbk = std::bind(&CollectionInterval::onDisplayRegistered, this, ph::_1, ph::_2, session);
    Utils::xmdsManager().registerDisplay(121, "1.8", "Display", clbk);
}

void CollectionInterval::startRegularCollection()
{
    collect(std::bind(&CollectionInterval::onCollectionFinished, this, ph::_1));
}

void CollectionInterval::sessionFinished(CollectionSessionPtr session)
{
    Glib::MainContext::get_default()->invoke([=](){
        session->callback(CollectionResult{true});
        return false;
    });

    session->listener.reset();
}

void CollectionInterval::onDisplayRegistered(const RegisterDisplay::Response::Status& status, const PlayerSettings& settings, CollectionSessionPtr session)
{
    displayMessage(status);
    if(status.code == RegisterDisplay::Response::Status::Code::Ready)
    {
//        updateTimer(settings.collectInterval);
        Utils::xmdsManager().requiredFiles(std::bind(&CollectionInterval::onRequiredFiles, this, ph::_1, ph::_2, session));
    }
    else
    {
        sessionFinished(session);
    }
}

void CollectionInterval::displayMessage(const RegisterDisplay::Response::Status& status)
{
    if(status.code != RegisterDisplay::Response::Status::Code::Invalid)
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

void CollectionInterval::onRequiredFiles(const RegularFiles& files, const ResourceFiles& resources, CollectionSessionPtr session)
{
    Log::debug("{} files and {} resources to download", files.size(), resources.size());

    session->listener = std::make_shared<AsyncListener>(std::bind(&CollectionInterval::sessionFinished, this, session));

    session->filesDownloader.download(files, session->listener->add<void>([](){
        Log::debug("Files downloaded");
    }));

    session->resourcesDownloader.download(resources, session->listener->add<void>([](){
        Log::debug("Resources downloaded");
    }));
}
