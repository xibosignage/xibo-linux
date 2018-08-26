#include "DownloadManager.hpp"
#include "constants.hpp"
#include "utils/utilities.hpp"
#include "xmds/SOAPManager.hpp"
#include "xmds/GetResource.hpp"

#include <regex>
#include <fstream>

DownloadManager::DownloadManager() :
    m_work{m_ioc}
{
    m_spdlog = spdlog::get(LOGGER);
    m_work_thread.reset(new std::thread([=](){
        m_ioc.run();
    }));
}

DownloadManager::~DownloadManager()
{
    m_ioc.stop();
    m_work_thread->join();
}

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
        m_spdlog->debug("host: {} target: {}", m_host, session->target);
    }

    auto resolve = std::bind(&DownloadManager::on_resolve, this, std::placeholders::_1, std::placeholders::_2, session);
    session->resolver.async_resolve(m_host, std::to_string(80), ip::resolver_base::numeric_service, resolve);
}

void DownloadManager::download(int layout_id, int region_id, int media_id, DownloadCallback callback)
{
    GetResource::request request;
    request.layout_id = layout_id;
    request.region_id = std::to_string(region_id);
    request.media_id = std::to_string(media_id);
    request.server_key = "egDeedO6";
    request.hardware_key = "Postman";

    utils::soap_manager().send_request(request, [media_id, callback](const GetResource::response& response){
        auto filename = "resources/" + std::to_string(media_id) + ".html";
        std::ofstream out(filename);
        out << response.resource;
        callback(filename);
    });
}

void DownloadManager::on_read(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<DownloadSession> session)
{
    //m_spdlog->debug("Read: {} error: {}", bytes_transferred, ec.value());

    std::ofstream out("resources/" + session->filename);
    out << session->http_response.body();

    session->callback("resources/" + session->filename);
}

void DownloadManager::on_write(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<DownloadSession> session)
{
    //m_spdlog->debug("Write: {} error: {}", bytes_transferred, ec.value());

    auto read = std::bind(&DownloadManager::on_read, this, std::placeholders::_1, std::placeholders::_2, session);
    http::async_read(session->socket, session->buffer, session->http_response, read);
}

void DownloadManager::on_connect(const boost::system::error_code& ec, ip::tcp::resolver::iterator, std::shared_ptr<DownloadSession> session)
{
    //m_spdlog->debug("Connected, error: {}", ec.value());

    session->http_request.method(http::verb::get);
    session->http_request.target(session->target);
    session->http_request.version(11);
    session->http_request.set(http::field::host, m_host);
    session->http_request.prepare_payload();

    auto write = std::bind(&DownloadManager::on_write, this, std::placeholders::_1, std::placeholders::_2, session);
    http::async_write(session->socket, session->http_request, write);
}


void DownloadManager::on_resolve(const boost::system::error_code& ec, ip::tcp::resolver::results_type results, std::shared_ptr<DownloadSession> session)
{
    //m_spdlog->debug("Resolved, error: {}", ec.value());

    auto connect = std::bind(&DownloadManager::on_connect, this, std::placeholders::_1, std::placeholders::_2, session);
    asio::async_connect(session->socket, results.begin(), results.end(), connect);
}
