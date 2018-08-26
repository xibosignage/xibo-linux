#include "CollectionInterval.hpp"
#include "constants.hpp"
#include "utils/utilities.hpp"

#include <glibmm/main.h>

CollectionInterval::CollectionInterval()
{
    m_logger = spdlog::get(LOGGER);
}

void CollectionInterval::start()
{
    Glib::signal_timeout().connect_seconds([=](){
        collect_data();
        return true;
    }, 20);

    collect_data();
}

void CollectionInterval::collect_data()
{
    auto clbk = std::bind(&CollectionInterval::on_register_display, this, std::placeholders::_1);
    utils::xmds_manager().register_display(121, "1.8", "Display", clbk);
}

void CollectionInterval::on_register_display(const RegisterDisplay::Response& response)
{
    switch(response.status)
    {
    case RegisterDisplay::Response::Status::Ready:
        m_logger->info("Display is ready. Getting required files...");
        utils::xmds_manager().required_files(std::bind(&CollectionInterval::on_required_files, this, std::placeholders::_1));
        break;
    case RegisterDisplay::Response::Status::Added:
        m_logger->info("Display has been added and waiting for approval in CMS");
        break;
    case RegisterDisplay::Response::Status::Waiting:
        m_logger->info("Displat is still waiting for approval in CMS");
        break;
    default:
        m_logger->critical("Invalid display status"); // FIXME exception(?)
        break;
    }
}

void CollectionInterval::on_required_files(const RequiredFiles::Response& response)
{
    m_logger->info("{} files and {} resources to download", response.required_files().size(), response.required_resources().size());

    auto clbk = std::bind(&CollectionInterval::download_callback, this, std::placeholders::_1);

    for(auto&& file : response.required_files())
    {
        m_logger->trace("File type: {} Id: {} Size: {}", (int)file.file_type, file.id, file.size);
        m_logger->trace("MD5: {} Filename: {} Download type: {}", file.md5, file.filename, (int)file.download_type);

        m_download_manager.download(file.filename, file.path, clbk); // FIXME add download type
    }

    for(auto&& resource : response.required_resources())
    {
        m_logger->trace("layout_id: {} region_id: {} media_id: {}", resource.layout_id, resource.region_id, resource.media_id);

        m_download_manager.download(resource.layout_id, resource.region_id, resource.media_id, clbk);
    }
}

void CollectionInterval::download_callback(const std::string& filename)
{
    m_logger->trace("{} downloaded", filename);
}

