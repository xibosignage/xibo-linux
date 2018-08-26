#include "CollectionInterval.hpp"

#include "constants.hpp"
#include "utils/utilities.hpp"
#include "DownloadManager.hpp"

#include <glibmm/main.h>

CollectionInterval::CollectionInterval()
{
    m_logger = spdlog::get(LOGGER);
}

void CollectionInterval::start()
{
    collect_data();
}

sigc::signal<void>& CollectionInterval::signal_finished()
{
    return m_signal_finished;
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
        m_logger->info("Display is still waiting for approval in CMS");
        break;
    default:
        m_logger->critical("Invalid display status"); // FIXME exception(?)
        break;
    }
}

void CollectionInterval::on_required_files(const RequiredFiles::Response& response)
{
    size_t files_count = response.required_files().size();
    size_t resources_count = response.required_resources().size();

    m_logger->debug("{} files and {} resources to download", files_count, resources_count);

    auto session = std::make_shared<RequiredFilesSession>();
    session->download_overall = files_count + resources_count;
    auto clbk = std::bind(&CollectionInterval::download_callback, this, std::placeholders::_1, session);

    for(auto&& file : response.required_files())
    {
        m_logger->trace("File type: {} Id: {} Size: {}", static_cast<int>(file.file_type), file.id, file.size);
        m_logger->trace("MD5: {} Filename: {} Download type: {}", file.md5, file.filename, static_cast<int>(file.download_type));

        utils::download_manager().download(file.filename, file.path, clbk); // FIXME add download type
    }

    for(auto&& resource : response.required_resources())
    {
        m_logger->trace("layout_id: {} region_id: {} media_id: {}", resource.layout_id, resource.region_id, resource.media_id);

        utils::download_manager().download(resource.layout_id, resource.region_id, resource.media_id, clbk);
    }
}

void CollectionInterval::download_callback(const std::string& filename, RequiredFilesSessionPtr session)
{
    m_logger->trace("{} downloaded", filename);
    if(++session->download_count == session->download_overall)
    {
        Glib::MainContext::get_default()->invoke([=](){
            m_logger->debug("All files downloaded");
            m_signal_finished.emit();
            return false;
        });
    }
}
