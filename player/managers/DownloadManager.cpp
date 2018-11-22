#include "DownloadManager.hpp"

#include "constants.hpp"
#include "xmds/XMDSManager.hpp"
#include "utils/Resources.hpp"
#include "utils/Utilities.hpp"

#include <regex>
#include <fstream>

DownloadManager::DownloadManager() :
    m_work{m_ioc}
{
    m_logger = spdlog::get(LOGGER);
    m_workThread.reset(new std::thread([=](){
        m_ioc.run();
    }));
}

DownloadManager::~DownloadManager()
{
    m_ioc.stop();
    m_workThread->join();
}

// FIXME GetFile should be used in case of XMDS download type
void DownloadManager::download(const std::string& filename, const std::string& path, DownloadCallback callback)
{
    auto session = std::make_shared<DownloadSession>(m_ioc);
    session->filename = filename;
    session->callback = callback;
    session->httpResponse.body_limit(std::numeric_limits<std::uint64_t>::max());

    const int GROUPS_COUNT = 3;
    std::regex urlRegex("([A-Za-z]*://)(.*)(/.*)");
    std::smatch baseMatch;
    if(std::regex_match(path, baseMatch, urlRegex) && baseMatch.size() > GROUPS_COUNT)
    {
        m_host = baseMatch[2].str();
        session->target = baseMatch[3].str();
        m_logger->trace("Host: {} Target: {}", m_host, session->target);
    }

    auto resolve = std::bind(&DownloadManager::onResolve, this, std::placeholders::_1, std::placeholders::_2, session);
    session->resolver.async_resolve(m_host, std::to_string(80), ip::resolver_base::numeric_service, resolve);
}

void DownloadManager::download(int layoutId, int regionId, int mediaId, DownloadCallback callback)
{
    Utils::xmdsManager().getResource(layoutId, regionId, mediaId, [=](const GetResource::Response& response){
        auto filename = Resources::directory() / (std::to_string(mediaId) + ".html");
        std::ofstream out(filename.string());
        out << response.resource;
        callback(filename.string());
    });
}

void DownloadManager::onRead(const boost::system::error_code& ec, std::size_t bytes, std::shared_ptr<DownloadSession> session)
{
    if(!ec)
    {
        auto filename = Resources::directory() / session->filename;
        std::ofstream out(filename.string());
        out << session->httpResponse.get().body();
        session->callback(filename.string());
        m_logger->debug("[{}] Downloaded {} bytes", session->filename, bytes);
    }
    else
    {
        m_logger->error("[{}] Download error: {}", session->filename, ec.message());
    }
}

void DownloadManager::onWrite(const boost::system::error_code& ec, std::size_t, std::shared_ptr<DownloadSession> session)
{
    if(!ec)
    {  
        auto read = std::bind(&DownloadManager::onRead, this, std::placeholders::_1, std::placeholders::_2, session);
        http::async_read(session->socket, session->buffer, session->httpResponse, read);
    }
    else
    {
        m_logger->error("[{}] Send download request error: {}", session->filename, ec.message());
    }
}

void DownloadManager::onConnect(const boost::system::error_code& ec, ip::tcp::resolver::iterator, std::shared_ptr<DownloadSession> session)
{
    if(!ec)
    {
        session->httpRequest.method(http::verb::get);
        session->httpRequest.target(session->target);
        session->httpRequest.version(11);
        session->httpRequest.set(http::field::host, m_host);
        session->httpRequest.prepare_payload();

        auto write = std::bind(&DownloadManager::onWrite, this, std::placeholders::_1, std::placeholders::_2, session);
        http::async_write(session->socket, session->httpRequest, write);
    }
    else
    {
        m_logger->error("[{}] Connected to host with error: {}", session->filename, ec.message());
    }
}

void DownloadManager::onResolve(const boost::system::error_code& ec, ip::tcp::resolver::results_type results, std::shared_ptr<DownloadSession> session)
{
    if(!ec)
    {
        auto connect = std::bind(&DownloadManager::onConnect, this, std::placeholders::_1, std::placeholders::_2, session);
        asio::async_connect(session->socket, results.begin(), results.end(), connect);
    }
    else
    {
        m_logger->error("[{}] Resolved host with error: {}", session->filename, ec.message());
    }
}
