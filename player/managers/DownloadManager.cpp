#include "DownloadManager.hpp"

#include "constants.hpp"
#include "utils/utilities.hpp"
#include "xmds/XMDSManager.hpp"

#include <regex>
#include <fstream>

const std::string DEFAULT_FOLDER = "LayerTest";

DownloadManager::DownloadManager() :
    m_work{m_ioc}
{
    m_logger = spdlog::get(LOGGER);
    m_workThread.reset(new std::thread([=](){
        m_ioc.run();
    }));

    init();
}

DownloadManager::~DownloadManager()
{
    m_ioc.stop();
    m_workThread->join();
}

void DownloadManager::init()
{
    m_resourcesDir = std::filesystem::current_path() / DEFAULT_FOLDER;
    if(!std::filesystem::exists(m_resourcesDir))
    {
        bool result = std::filesystem::create_directory(m_resourcesDir);
        if(!result)
        {
            throw std::runtime_error("Unable to create resources directory");
        }
        else
        {
            m_logger->info("Resource directory is {}", m_resourcesDir.string());
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
    utils::xmdsManager().getResource(layoutId, regionId, mediaId, [=](const GetResource::Response& response){
        auto filename = resourcesDir() / (std::to_string(mediaId) + ".html");
        std::ofstream out(filename.string());
        out << response.resource;
        callback(filename.string());
    });
}

std::filesystem::path DownloadManager::resourcesDir()
{
    return m_resourcesDir;
}

void DownloadManager::onRead(const boost::system::error_code& ec, std::size_t, std::shared_ptr<DownloadSession> session)
{
    if(!ec)
    {
        auto filename = resourcesDir() / session->filename;
        std::ofstream out(filename.string());
        out << session->httpResponse.body();
        session->callback(filename.string());
    }
    else
    {
        m_logger->error("[{}] Download error: {}", session->filename, ec.message());
        m_logger->warn("Current the player doesn't support VERY big files like video/audio (will be fixed in the next release)");
    }
}

#include <limits>
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
