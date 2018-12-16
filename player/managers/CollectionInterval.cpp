#include "CollectionInterval.hpp"

#include "xmds/XMDSManager.hpp"

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
    Utils::logger()->debug("Collection finished with success result: {}", result.success);
    Utils::logger()->debug("Next collection will start in {} seconds", m_collectInterval);
    startTimer();
}

void CollectionInterval::collect(CollectionResultCallback callback)
{
    Utils::logger()->debug("Collection started");

    auto session = std::make_shared<CollectionSession>();
    session->callback = callback;

    auto clbk = std::bind(&CollectionInterval::onDisplayRegistered, this, ph::_1, ph::_2, session);
    Utils::xmdsManager().registerDisplay(121, "1.8", "Display", clbk);
}

void CollectionInterval::startRegularCollection()
{
    collect(std::bind(&CollectionInterval::onCollectionFinished, this, ph::_1));
}

void CollectionInterval::onDisplayRegistered(const RegisterDisplay::Response::Status& status, const PlayerSettings& settings, CollectionSessionPtr session)
{
    switch(status.code)
    {
    case RegisterDisplay::Response::Status::Code::Ready:
        updateTimer(settings.collectInterval);
        Utils::xmdsManager().requiredFiles(std::bind(&CollectionInterval::onRequiredFiles, this, ph::_1, ph::_2, session));
        Utils::logger()->debug("Display is ready. Getting required files and resources...");
        break;
    case RegisterDisplay::Response::Status::Code::Added:
        Utils::logger()->debug("Display has been added and waiting for approval in CMS");
        break;
    case RegisterDisplay::Response::Status::Code::Waiting:
        Utils::logger()->debug("Display is waiting for approval in CMS");
        break;
    default:
        Utils::logger()->critical("Invalid display status"); // WARNING exception(?)
        break;
    }
}

void CollectionInterval::updateTimer(int collectInterval)
{
    if(m_collectInterval != collectInterval)
    {
        Utils::logger()->debug("Collection interval updated. Old: {}, New: {}", m_collectInterval, collectInterval);
        m_collectInterval = collectInterval;
    }
}

void CollectionInterval::onRequiredFiles(const RegularFiles& files, const ResourceFiles& resources, CollectionSessionPtr session)
{
    Utils::logger()->debug("{} files and {} resources to download", files.size(), resources.size());

    m_downloader.download(files, resources, [=](){
        Utils::logger()->debug("All files downloaded");
    });
}
