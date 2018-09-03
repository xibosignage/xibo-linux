#ifndef DOWNLOADMANAGER_HPP
#define DOWNLOADMANAGER_HPP

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem/path.hpp>
#include <spdlog/spdlog.h>

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;
namespace ip = boost::asio::ip;

using DownloadCallback = std::function<void(const std::string&)>;

struct DownloadSession
{
    DownloadSession(asio::io_context& ioc) : socket(ioc), resolver(ioc) { }
    ip::tcp::socket socket;
    ip::tcp::resolver resolver;
    http::request<http::string_body> httpRequest;
    http::response<http::string_body> httpResponse;
    beast::flat_buffer buffer;
    std::string filename;
    std::string target;
    std::function<void(const std::string&)> callback;
};

class DownloadManager
{
public:
    DownloadManager();
    ~DownloadManager();
    DownloadManager(const DownloadManager&) = delete;
    DownloadManager& operator=(const DownloadManager&) = delete;

    void download(const std::string& filename, const std::string& path, DownloadCallback callback);
    void download(int layoutId, int regionId, int mediaId, DownloadCallback callback);
    boost::filesystem::path resourcesDir();

private:
    void init();

    void onRead(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<DownloadSession> session);
    void onWrite(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<DownloadSession> session);
    void onConnect(const boost::system::error_code& ec, ip::tcp::resolver::iterator, std::shared_ptr<DownloadSession> session);
    void onResolve(const boost::system::error_code& ec, ip::tcp::resolver::results_type results, std::shared_ptr<DownloadSession> session);

private:
    asio::io_context m_ioc;
    asio::io_context::work m_work;
    std::unique_ptr<std::thread> m_workThread;
    std::string m_host;
    std::shared_ptr<spdlog::logger> m_logger;
    boost::filesystem::path m_resourcesDir;
};


#endif // DOWNLOADMANAGER_HPP
