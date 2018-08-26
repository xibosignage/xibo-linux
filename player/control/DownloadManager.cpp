#include "DownloadManager.hpp"

#include "constants.hpp"
#include "utils/utilities.hpp"
#include "xmds/XMDSManager.hpp"

#include <regex>
#include <fstream>
#include <boost/filesystem.hpp>

const std::string DEFAULT_FOLDER = "resources";

DownloadManager::DownloadManager() :
    m_work{m_ioc}
{
    m_logger = spdlog::get(LOGGER);
    m_work_thread.reset(new std::thread([=](){
        m_ioc.run();
    }));

    init();
}

DownloadManager::~DownloadManager()
{
    m_ioc.stop();
    m_work_thread->join();
}

void DownloadManager::init()
{
    m_resources_dir = utils::app_current_dir() / DEFAULT_FOLDER;
    if(!boost::filesystem::exists(m_resources_dir))
    {
        bool result = boost::filesystem::create_directory(m_resources_dir);
        if(!result)
        {
            throw std::runtime_error("Unable to create resources directory");
        }
        else
        {
            m_logger->info("Resource directory is {}", m_resources_dir.string());
        }
    }
}

// FIXME GetFile should be used in case of XMDS download type
void DownloadManager::download(const std::string& filename, const std::string& path, DownloadCallback callback)
{
    auto session = std::make_shared<DownloadSession>(m_ioc);
    session->filename = filename;
    session->callback = callback;

    const int GROUPS_COUNT = 3;
    std::regex url_regex("([A-Za-z]*://)(.*)(/.*)");
    std::smatch base_match;
    if(std::regex_match(path, base_match, url_regex) && base_match.size() > GROUPS_COUNT)
    {
        m_host = base_match[2].str();
        session->target = base_match[3].str();
        m_logger->trace("Host: {} Target: {}", m_host, session->target);
    }

    auto resolve = std::bind(&DownloadManager::on_resolve, this, std::placeholders::_1, std::placeholders::_2, session);
    session->resolver.async_resolve(m_host, std::to_string(80), ip::resolver_base::numeric_service, resolve);
}

void DownloadManager::download(int layout_id, int region_id, int media_id, DownloadCallback callback)
{
    utils::xmds_manager().get_resource(layout_id, region_id, media_id, [=](const GetResource::Response& response){
        auto filename = "resources/" + std::to_string(media_id) + ".html";
        std::ofstream out(filename);
        out << response.resource;
        callback(filename);
    });
}

boost::filesystem::path DownloadManager::resources_dir()
{
    return m_resources_dir;
}

void DownloadManager::on_read(const boost::system::error_code& ec, std::size_t, std::shared_ptr<DownloadSession> session)
{
    if(!ec)
    {
        auto filename = "resources/" + session->filename;
        std::ofstream out(filename);
        out << session->http_response.body();
        session->callback(filename);
    }
    else
    {
        m_logger->debug("[{}] Download error: {}", session->filename, ec.message());
    }
}

void DownloadManager::on_write(const boost::system::error_code& ec, std::size_t, std::shared_ptr<DownloadSession> session)
{
    if(!ec)
    {
        auto read = std::bind(&DownloadManager::on_read, this, std::placeholders::_1, std::placeholders::_2, session);
        http::async_read(session->socket, session->buffer, session->http_response, read);
    }
    else
    {
        m_logger->debug("[{}] Send download request error: {}", session->filename, ec.message());
    }
}

void DownloadManager::on_connect(const boost::system::error_code& ec, ip::tcp::resolver::iterator, std::shared_ptr<DownloadSession> session)
{
    if(!ec)
    {
        session->http_request.method(http::verb::get);
        session->http_request.target(session->target);
        session->http_request.version(11);
        session->http_request.set(http::field::host, m_host);
        session->http_request.prepare_payload();

        auto write = std::bind(&DownloadManager::on_write, this, std::placeholders::_1, std::placeholders::_2, session);
        http::async_write(session->socket, session->http_request, write);
    }
    else
    {
        m_logger->debug("[{}] Connected to host with error: {}", session->filename, ec.message());
    }
}

void DownloadManager::on_resolve(const boost::system::error_code& ec, ip::tcp::resolver::results_type results, std::shared_ptr<DownloadSession> session)
{
    if(!ec)
    {
        auto connect = std::bind(&DownloadManager::on_connect, this, std::placeholders::_1, std::placeholders::_2, session);
        asio::async_connect(session->socket, results.begin(), results.end(), connect);
    }
    else
    {
        m_logger->debug("[{}] Resolved host with error: {}", session->filename, ec.message());
    }
}
