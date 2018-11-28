#ifndef DOWNLOADMANAGER_HPP
#define DOWNLOADMANAGER_HPP

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <thread>

using DownloadCallback = std::function<void(const std::string&)>;

struct DownloadSession
{
    DownloadSession(boost::asio::io_context& ioc) : socket(ioc), resolver(ioc) { }
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver resolver;
    boost::beast::http::request<boost::beast::http::string_body> httpRequest;
    boost::beast::http::response_parser<boost::beast::http::string_body> httpResponse;
    boost::beast::flat_buffer buffer;
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

private:
    void onRead(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<DownloadSession> session);
    void onWrite(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<DownloadSession> session);
    void onConnect(const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator, std::shared_ptr<DownloadSession> session);
    void onResolve(const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::results_type results, std::shared_ptr<DownloadSession> session);

private:
    boost::asio::io_context m_ioc;
    boost::asio::io_context::work m_work;
    std::unique_ptr<std::thread> m_workThread;
    std::string m_host;
};


#endif // DOWNLOADMANAGER_HPP
