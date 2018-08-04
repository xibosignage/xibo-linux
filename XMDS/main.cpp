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

template<typename request, typename callback>
void send_request(const request& soap_request, callback response_callback)
{
    namespace http = boost::beast::http;
    namespace asio = boost::asio;
    namespace ip = boost::asio::ip;
    boost::asio::io_context ioc;
    ip::tcp::socket socket{ioc};
    ip::tcp::resolver resolver{ioc};
    auto results = resolver.resolve("linuxplayer.xibo.co.uk", "80", ip::resolver_base::numeric_service);
    asio::connect(socket, results.begin(), results.end());

    http::request<http::string_body> http_request;
    http_request.method(http::verb::post);
    http_request.target("/xmds.php?v=5");
    http_request.version(11);
    http_request.set(http::field::host, "linuxplayer.xibo.co.uk");
    http_request.body() = soap::request_string(soap_request);
    http_request.prepare_payload();

    std::cout << soap::request_string(soap_request) << std::endl;

    http::write(socket, http_request);

    http::response<http::string_body> http_response;
    boost::beast::flat_buffer buffer;
    http::read(socket, buffer, http_response);

    using response = typename soap::request_traits<request>::response_t;
    auto result = soap::create_response<response>(http_response.body());
    response_callback(result);
}

int main()
{
    {
        RegisterDisplay::request request;
        request.display_name = "MyDisplay";
        request.client_type = "linux";
        request.client_version = "1.8";
        request.client_code = 1;
        request.mac_address = "MyAddress";
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";

        send_request(request, [=](const RegisterDisplay::response& response){
            std::cout << response.display_name << " " << response.collect_interval << std::endl;
        });
    }

    {
        RequiredFiles::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";

        send_request(request, [=](const RequiredFiles::response& response){
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

        send_request(request, [=](const GetFile::response& response){
            std::cout << response.base64chunk << std::endl;
        });
    }

    {
        NotifyStatus::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";
        request.status = "test";

        send_request(request, [=](const NotifyStatus::response& response){
            std::cout << response.success << std::endl;
        });
    }

    {
        MediaInventory::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";
        request.media_inventory = "test";

        send_request(request, [=](const MediaInventory::response& response){
            std::cout << response.success << std::endl;
        });
    }

    {
        SubmitLog::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";
        request.log_xml = "log";

        send_request(request, [=](const SubmitLog::response& response){
            std::cout << response.success << std::endl;
        });
    }

    {
        SubmitStats::request request;
        request.server_key = "egDeedO6";
        request.hardware_key = "MyTest";
        request.stat_xml = "stat";

        send_request(request, [=](const SubmitStats::response& response){
            std::cout << response.success << std::endl;
        });
    }

    return 0;
}
