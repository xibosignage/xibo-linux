#include "CollectionInterval.hpp"
#include "DownloadManager.hpp"

#include "constants.hpp"

#include "utils/utilities.hpp"
#include "control/PlayerSettings.hpp"

#include <glibmm/main.h>

const uint DEFAULT_INTERVAL = 60;

// FIXME interval may not be finished before starting the new one
CollectionInterval::CollectionInterval()
{
    m_logger = spdlog::get(LOGGER);
}

void CollectionInterval::start()
{
    // NOTE: doesn't actually run 20 seconds
//    m_logger->debug("CollectionInterval timer started with DEFAULT {} interval", DEFAULT_INTERVAL);
    m_intervalConnection = Glib::signal_timeout().connect_seconds([=]{
        collectData();
        return true;
    }, DEFAULT_INTERVAL);

    collectData();
}

sigc::signal<void>& CollectionInterval::signalFinished()
{
    return m_signalFinished;
}

sigc::signal<void, PlayerSettings>&CollectionInterval::signalSettingsUpdated()
{
    return m_signalSettingsUpdated;
}

void CollectionInterval::collectData()
{
    auto clbk = std::bind(&CollectionInterval::onRegisterDisplay, this, std::placeholders::_1);
    utils::xmdsManager().registerDisplay(121, "1.8", "Display", clbk);
}

void CollectionInterval::updateTimer(uint collectInterval)
{
    if(m_collectInterval != collectInterval)
    {
        m_collectInterval = collectInterval;
        m_intervalConnection.disconnect();

        m_intervalConnection = Glib::signal_timeout().connect_seconds([=]{
//            m_logger->debug("CollectionInterval timer started with {} interval", m_collect_interval.value());
            collectData();
            return true;
        }, m_collectInterval.value());
    }
}

void CollectionInterval::onRegisterDisplay(const RegisterDisplay::Response& response)
{
    switch(response.status)
    {
    case RegisterDisplay::Response::Status::Ready:
        m_logger->debug("Display is ready. Getting required files...");
        updateTimer(response.playerSettings.collectInterval);
        m_signalSettingsUpdated.emit(response.playerSettings);
        utils::xmdsManager().requiredFiles(std::bind(&CollectionInterval::onRequiredFiles, this, std::placeholders::_1));
        break;
    case RegisterDisplay::Response::Status::Added:
        m_logger->debug("Display has been added and waiting for approval in CMS");
        break;
    case RegisterDisplay::Response::Status::Waiting:
        m_logger->debug("Display is still waiting for approval in CMS");
        break;
    default:
        m_logger->critical("Invalid display status"); // WARNING exception(?)
        break;
    }
}

void CollectionInterval::onRequiredFiles(const RequiredFiles::Response& response)
{
    size_t filesCount = response.requiredFiles().size();
    size_t resourcesCount = response.requiredResources().size();

    m_logger->debug("{} files and {} resources to download", filesCount, resourcesCount);

    auto session = std::make_shared<RequiredFilesSession>();
    session->downloadOverall = filesCount + resourcesCount;
    auto clbk = std::bind(&CollectionInterval::downloadCallback, this, std::placeholders::_1, session);

    for(auto&& file : response.requiredFiles())
    {
        m_logger->trace("File type: {} Id: {} Size: {}", static_cast<int>(file.fileType), file.id, file.size);
        m_logger->trace("MD5: {} Filename: {} Download type: {}", file.md5, file.filename, static_cast<int>(file.downloadType));

        utils::downloadManager().download(file.filename, file.path, clbk); // FIXME add download type
    }

    for(auto&& resource : response.requiredResources())
    {
        m_logger->trace("layout_id: {} region_id: {} media_id: {}", resource.layoutId, resource.regionId, resource.mediaId);

        utils::downloadManager().download(resource.layoutId, resource.regionId, resource.mediaId, clbk);
    }
}

void CollectionInterval::downloadCallback(const std::string& filename, RequiredFilesSessionPtr session)
{
    m_logger->debug("{} downloaded", filename);
    if(++session->downloadCount == session->downloadOverall)
    {
        Glib::MainContext::get_default()->invoke([=](){
            m_logger->debug("All files downloaded");
            m_signalFinished.emit();
            return false;
        });
    }
}
