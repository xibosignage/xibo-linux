#include <iostream>
#include <functional>
#include <string>

#include <boost/beast.hpp>
#include <boost/asio.hpp>

#include "RegisterDisplay.hpp"
#include "RequiredFiles.hpp"
#include "GetFile.hpp"
#include "NotifyStatus.hpp"
#include "MediaInventory.hpp"
#include "SubmitLog.hpp"
#include "SubmitStats.hpp"

namespace http = boost::beast::http;
namespace asio = boost::asio;
namespace ip = boost::asio::ip;
using namespace std::chrono_literals;

struct Session
{
    Session(boost::asio::io_context& ioc) : socket(ioc), resolver(ioc) { }
    ip::tcp::socket socket;
    ip::tcp::resolver resolver;
    http::request<http::string_body> http_request;
    boost::beast::flat_buffer buffer;
    http::response<http::string_body> http_response;
};

template<typename request, typename callback>
void send_request(boost::asio::io_context& ioc, const request& soap_request, callback&& response_callback)
{
    auto session = std::make_shared<Session>(ioc);
    session->resolver.async_resolve("linuxplayer.xibo.co.uk", "80", ip::resolver_base::numeric_service, [=](const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::results_type results){
        std::cout << "resolved " << ec.value() << std::endl;
        asio::async_connect(session->socket, results.begin(), results.end(), [=](const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator){
            std::cout << "connected " << ec.value() << std::endl;

            session->http_request.method(http::verb::post);
            session->http_request.target("/xmds.php?v=5");
            session->http_request.version(11);
            session->http_request.set(http::field::host, "linuxplayer.xibo.co.uk");
            session->http_request.body() = soap::request_string(soap_request);
            session->http_request.prepare_payload();

            std::cout << soap::request_string(soap_request) << std::endl;

            auto write = std::bind([=](const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<Session> session){
                std::cout << "Write: " << bytes_transferred << " error: " << ec.value() << std::endl;

                auto read = std::bind([=](const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<Session> session){
                    std::cout << "Read: " << bytes_transferred << " error: " << ec.value() << std::endl;

                    using response = typename soap::request_traits<request>::response_t;
                    auto result = soap::create_response<response>(session->http_response.body());
                    response_callback(result);
                }, std::placeholders::_1, std::placeholders::_2, session);
                http::async_read(session->socket, session->buffer, session->http_response, read);

            }, std::placeholders::_1, std::placeholders::_2, session);

            http::async_write(session->socket, session->http_request, write);
        });
    });
}

int main()
{
    boost::asio::io_context ioc;
    boost::asio::io_context::work work{ioc};

    std::thread t([&](){
        ioc.run();
    });

    {
        RegisterDisplay::request request;
        request.display_name = "MyDisplay";
        request.client_type = "linux";
        request.client_version = "1.8";
        request.client_code = 1;
        request.mac_address = "MyAddress";
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";

        send_request(ioc, request, [](const RegisterDisplay::response& response){
            std::cout << response.display_name << " " << response.collect_interval << std::endl;
        });
    }

    {
        RequiredFiles::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";

        send_request(ioc, request, [](const RequiredFiles::response& response){
            std::cout << response.required_files().size() << std::endl;
            for(auto&& file : response.required_files())
            {
                std::cout << "file type: " << (int)file.file_type << " id: " << file.id << " size: " << file.size << std::endl;
                std::cout << "md5: " << file.md5 << " filename: " << file.filename << " download type: " << (int)file.download_type << std::endl;
                std::cout << "path: " << file.path << std::endl << std::endl;
            }
        });
    }

    {
        GetFile::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";
        request.file_id = std::to_string(201);
        request.file_type = "media";
        request.chunk_offset = 0;
        request.chunk_size = 1000;

        send_request(ioc, request, [](const GetFile::response& response){
            std::cout << response.base64chunk << std::endl;
        });
    }

    {
        NotifyStatus::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";
        request.status = "test";

        send_request(ioc, request, [](const NotifyStatus::response& response){
            std::cout << response.success << std::endl;
        });
    }

    {
        MediaInventory::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";
        request.media_inventory = "test";

        send_request(ioc, request, [](const MediaInventory::response& response){
            std::cout << response.success << std::endl;
        });
    }

    {
        SubmitLog::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";
        request.log_xml = "log";

        send_request(ioc, request, [](const SubmitLog::response& response){
            std::cout << response.success << std::endl;
        });
    }

    {
        SubmitStats::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";
        request.stat_xml = "stat";

        send_request(ioc, request, [](const SubmitStats::response& response){
            std::cout << response.success << std::endl;
        });
    }

    std::this_thread::sleep_for(5s);
    ioc.stop();
    t.join();

    return 0;
}
