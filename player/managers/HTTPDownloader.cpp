#include "HTTPDownloader.hpp"

#include "utils/Resources.hpp"
#include "utils/Logger.hpp"

#include <regex>
#include <fstream>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/asio/connect.hpp>

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;
namespace ip = boost::asio::ip;

const std::regex URL_REGEX("([A-Za-z]*://)(.*)(/.*)");

struct DownloadSession
{
    DownloadSession(boost::asio::io_context& ioc) : socket(ioc), resolver(ioc)
    {
        httpResponse.body_limit(std::numeric_limits<std::uint64_t>::max());
    }

    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver resolver;
    boost::beast::http::request<boost::beast::http::string_body> httpRequest;
    boost::beast::http::response_parser<boost::beast::http::string_body> httpResponse;
    boost::beast::flat_buffer buffer;
    std::string filename;
    std::string target;
    std::string host;
    FileDownloadCallback callback;
    std::promise<void> result;
};

HTTPDownloader::HTTPDownloader() :
    m_work{m_ioc}
{
    m_workThread = std::make_unique<JoinableThread>([=](){
        m_ioc.run();
    });
}

HTTPDownloader::~HTTPDownloader()
{
    m_ioc.stop();
}

std::future<void> HTTPDownloader::download(const std::string& filename, const std::string& path, FileDownloadCallback callback)
{
    auto session = std::make_shared<DownloadSession>(m_ioc);
    session->filename = filename;
    session->callback = callback;

    const int GROUPS_COUNT = 3;
    std::smatch baseMatch;

    if(std::regex_match(path, baseMatch, URL_REGEX) && baseMatch.size() > GROUPS_COUNT)
    {
        session->host = baseMatch[2].str();
        session->target = baseMatch[3].str();
        Log::trace("Host: {} Target: {}", session->host, session->target);
    }

    auto resolve = std::bind(&HTTPDownloader::onResolve, this, std::placeholders::_1, std::placeholders::_2, session);
    session->resolver.async_resolve(session->host, std::to_string(80), ip::resolver_base::numeric_service, resolve);

    return session->result.get_future();
}

void HTTPDownloader::sessionFinished(const boost::system::error_code& ec, DownloadSessionPtr session)
{
    if(ec)
    {
        Log::trace("[{}] Session finished with error: {}", session->filename, ec.message());
    }
    session->callback(DownloadedFile{ec, session->filename});
    session->result.set_value();
}

void HTTPDownloader::onRead(const boost::system::error_code& ec, std::size_t bytes, DownloadSessionPtr session)
{
    auto filename = Resources::directory() / session->filename;
    if(!ec)
    {
        // FIXME it should use FileCacheManager
        if(!std::filesystem::exists(filename.string()))
        {
            std::ofstream out(filename.string());
            out << session->httpResponse.get().body();
        }
    }
    sessionFinished(ec, session);
}

void HTTPDownloader::onWrite(const boost::system::error_code& ec, std::size_t, DownloadSessionPtr session)
{
    if(!ec)
    {  
        auto read = std::bind(&HTTPDownloader::onRead, this, std::placeholders::_1, std::placeholders::_2, session);
        http::async_read(session->socket, session->buffer, session->httpResponse, read);
    }
    else
    {
        sessionFinished(ec, session);
    }
}

void HTTPDownloader::onConnect(const boost::system::error_code& ec, ip::tcp::resolver::iterator, DownloadSessionPtr session)
{
    if(!ec)
    {
        session->httpRequest.method(http::verb::get);
        session->httpRequest.target(session->target);
        session->httpRequest.version(11);
        session->httpRequest.set(http::field::host, session->host);
        session->httpRequest.prepare_payload();

        auto write = std::bind(&HTTPDownloader::onWrite, this, std::placeholders::_1, std::placeholders::_2, session);
        http::async_write(session->socket, session->httpRequest, write);
    }
    else
    {
        sessionFinished(ec, session);
    }
}

void HTTPDownloader::onResolve(const boost::system::error_code& ec, ip::tcp::resolver::results_type results, DownloadSessionPtr session)
{
    if(!ec)
    {
        auto connect = std::bind(&HTTPDownloader::onConnect, this, std::placeholders::_1, std::placeholders::_2, session);
        asio::async_connect(session->socket, results.begin(), results.end(), connect);
    }
    else
    {
        sessionFinished(ec, session);
    }
}
