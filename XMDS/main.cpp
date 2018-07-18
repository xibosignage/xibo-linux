#include <iostream>
#include <functional>
#include <string>

#include <boost/beast.hpp>
#include <boost/asio.hpp>

#include "Requests/RegisterDisplayRequest.hpp"
#include "Responses/RegisterDisplayResponse.hpp"
#include "Requests/RequiredFilesRequest.hpp"
#include "Responses/RequiredFilesResponse.hpp"
#include "Requests/NotifyStatusRequest.hpp"
#include "Requests/GetFileRequest.hpp"
#include "Responses/GetFileResponse.hpp"
#include "Requests/SubmitLogRequest.hpp"
#include "Requests/SubmitStatsRequest.hpp"
#include "Responses/SuccessResponse.hpp"

const char REGISTER_DISPLAY[] = "RegisterDisplay";
const char REQUIRED_FILES[] = "RequiredFiles";
const char SCHEDULE[] = "Schedule";
const char NOTIFY_STATUS[] = "NotifyStatus";
const char GET_RESOURCE[] = "GetResource";
const char GET_FILE[] = "GetFile";
const char SUBMIT_LOG[] = "SubmitLog";
const char SUBMIT_STATS[] = "SubmitStats";

template<typename Request, typename Callback>
void send_request(const Request& my_request, Callback callback)
{
    namespace http = boost::beast::http;
    namespace asio = boost::asio;
    namespace ip = boost::asio::ip;
    boost::asio::io_context ioc;
    ip::tcp::socket socket{ioc};
    ip::tcp::resolver resolver{ioc};
    auto results = resolver.resolve("linuxplayer.xibo.co.uk", "80", ip::resolver_base::numeric_service);
    asio::connect(socket, results.begin(), results.end());

    http::request<http::string_body> request;
    request.method(http::verb::post);
    request.target("/xmds.php?v=5");
    request.version(11);
    request.set(http::field::host, "linuxplayer.xibo.co.uk");
    request.body() = my_request.str();
    request.prepare_payload();

    std::cout << my_request.str() << std::endl;

    http::write(socket, request);

    http::response<http::string_body> response;
    boost::beast::flat_buffer buffer;
    http::read(socket, buffer, response);

    callback(typename Request::ResponseType{response.body()});
}

int main()
{
    {
        RegisterDisplayRequest request;
        request.server_key("egDeedO6");
        request.hardware_key("MyTest");
        request.display_name("MyDisplay");
        request.client_type("linux");
        request.client_version("1.8");
        request.client_code(121);
        request.mac_address("MyAddress");

        send_request(request, [=](const RegisterDisplayResponse& response){
            std::cout << "Status: " << (int)response.status() << ", message: " << response.status_message() << std::endl;
        });
    }

    {
        RequiredFilesRequest request;
        request.server_key("egDeedO6");
        request.hardware_key("MyTest");

        send_request(request, [=](const RequiredFilesResponse& response){
            std::cout << response.required_files().size() << std::endl;
            for(auto&& file : response.required_files())
            {
                std::cout << "type: " << file.type() << " id: " << file.id() << " size: " << file.size() << std::endl;
                std::cout << "md5: " << file.md5() << " filename: " << file.filename() << " download: " << file.download() << std::endl;
                std::cout << "path: " << file.path() << std::endl << std::endl;
            }
        });
    }

    {
        GetFileRequest request;
        request.server_key("egDeedO6");
        request.hardware_key("MyTest");
        request.file_id(201);
        request.file_type("media");
        request.chunk_offset(0);
        request.chunk_size(1000);

        send_request(request, [=](const GetFileResponse& response){
        });
    }

    {
        NotifyStatusRequest request;
        request.server_key("egDeedO6");
        request.hardware_key("MyTest");
        request.status("test");

        send_request(request, [=](const NotifyStatusResponse& response){
            std::cout << response.success() << std::endl;
        });
    }

    {
        SubmitLogRequest request;
        request.server_key("egDeedO6");
        request.hardware_key("test");
        request.log_xml("log");

        send_request(request, [=](const SubmitLogResponse& response){
            std::cout << response.success() << std::endl;
        });
    }

    {
        SubmitStatsRequest request;
        request.server_key("egDeedO6");
        request.hardware_key("MyTest");
        request.stat_xml("stat");

        send_request(request, [=](const SubmitStatsResponse& response){
            std::cout << response.success() << std::endl;
        });
    }

    return 0;
}
