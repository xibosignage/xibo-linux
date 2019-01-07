#include "CollectionInterval.hpp"

#include "xmds/XMDSManager.hpp"

#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "utils/TimerProvider.hpp"

#include <glibmm/main.h>

const uint DEFAULT_INTERVAL = 5;
namespace ph = std::placeholders;

#include <future>

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
    Log::debug("Collection started {}", std::this_thread::get_id());

    auto session = std::make_shared<CollectionSession>();
    session->callback = callback;

    auto registerDisplayResult = Utils::xmdsManager().registerDisplay(121, "1.8", "Display");
    onDisplayRegistered(registerDisplayResult.get(), session);
}

void CollectionInterval::startRegularCollection()
{
//    collect(std::bind(&CollectionInterval::onCollectionFinished, this, ph::_1));
}

void CollectionInterval::sessionFinished(CollectionSessionPtr session)
{
    Glib::MainContext::get_default()->invoke([=](){
        session->callback(CollectionResult{true});
        return false;
    });
}

void CollectionInterval::onDisplayRegistered(const RegisterDisplay::Response& response, CollectionSessionPtr session)
{
    Log::trace("onDisplayRegistered {}", std::this_thread::get_id());

    displayMessage(response.status);
    if(response.status.code == RegisterDisplay::Response::Status::Code::Ready)
    {
//        updateTimer(settings.collectInterval);

        auto requiredFilesResult = Utils::xmdsManager().requiredFiles();
        auto scheduleResult = Utils::xmdsManager().schedule();
        onSchedule(scheduleResult.get(), session);
        onRequiredFiles(requiredFilesResult.get(), session);

        sessionFinished(session);
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

void CollectionInterval::onRequiredFiles(const RequiredFiles::Response& response, CollectionSessionPtr)
{
    RequiredFilesDownloader filesDownloader;
    RequiredResourcesDownloader resourcesDownloader;

    auto&& files = response.requiredFiles;
    auto&& resources = response.requiredResources;

    Log::debug("{} files and {} resources to download {}", files.size(), resources.size(), std::this_thread::get_id());

    auto resourcesResult = resourcesDownloader.download(resources);
    auto filesResult = filesDownloader.download(files);

    Log::trace("Waiting for downloads... {}", std::this_thread::get_id());
    filesResult.wait();
    Log::debug("Files downloaded");
    resourcesResult.wait();
    Log::debug("Resources downloaded");
}

void CollectionInterval::onSchedule(const Schedule::Response& response, CollectionSessionPtr)
{
    Log::trace("OnSchedule {}", std::this_thread::get_id());

    Log::debug("Default layout Id: {}", response.defaultLayout.id);
    for(auto dependant : response.defaultLayout.dependants)
    {
        Log::debug(dependant);
    }

    for(auto layout : response.layouts)
    {
        Log::debug("ScheduleId: {} LayoutId: {} StartDT: {} EndDT: {} Priority: {}", layout.scheduleId, layout.id, layout.startDT, layout.endDT, layout.priority);
        for(auto dependant : layout.dependants)
        {
            Log::debug(dependant);
        }
    }

    Log::debug("Global dependants");

    for(auto dependant : response.globalDependants)
    {
        Log::debug(dependant);
    }
}
