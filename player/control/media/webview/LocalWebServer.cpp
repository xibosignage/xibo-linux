#include "LocalWebServer.hpp"

#include "common/logger/Logging.hpp"

const std::string XiboLocalWebServer = "Xibo Local WebSerbver";
const std::string DefaultLocalAddress = "127.0.0.1";
const int DefaultThreadsCount = 2;

beast::string_view mimeType(const FilePath& path)
{
    using beast::iequals;
    auto const extension = [path = path.string()] {
        auto const pos = path.rfind(".");
        if (pos == beast::string_view::npos) return std::string{};
        return path.substr(pos);
    }();

    if (extension == ".htm") return "text/html";
    if (extension == ".html") return "text/html";
    if (extension == ".php") return "text/html";
    if (extension == ".css") return "text/css";
    if (extension == ".txt") return "text/plain";
    if (extension == ".js") return "application/javascript";
    if (extension == ".json") return "application/json";
    if (extension == ".xml") return "application/xml";

    return "application/text";
}

template <class Body, class Allocator, class Send>
void handleRequest(const FilePath& rootDir, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send)
{
    auto const badRequest = [&req](beast::string_view why) {
        http::response<http::string_body> response{http::status::bad_request, req.version()};
        response.set(http::field::server, XiboLocalWebServer);
        response.set(http::field::content_type, "text/html");
        response.keep_alive(req.keep_alive());
        response.body() = std::string(why);
        response.prepare_payload();
        return response;
    };

    auto const notFound = [&req](beast::string_view target) {
        http::response<http::string_body> response{http::status::not_found, req.version()};
        response.set(http::field::server, XiboLocalWebServer);
        response.set(http::field::content_type, "text/html");
        response.keep_alive(req.keep_alive());
        response.body() = "The resource '" + std::string(target) + "' was not found.";
        response.prepare_payload();
        return response;
    };

    auto const serverError = [&req](beast::string_view what) {
        http::response<http::string_body> response{http::status::internal_server_error, req.version()};
        response.set(http::field::server, XiboLocalWebServer);
        response.set(http::field::content_type, "text/html");
        response.keep_alive(req.keep_alive());
        response.body() = "An error occurred: '" + std::string(what) + "'";
        response.prepare_payload();
        return response;
    };

    if (req.method() != http::verb::get) return send(badRequest("WebServer supports only GET requests"));

    if (req.target().empty() || req.target()[0] != '/' || req.target().find("..") != beast::string_view::npos)
        return send(badRequest("Illegal request-target"));

    FilePath path{rootDir.string() + std::string{req.target()}};

    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);

    if (ec == beast::errc::no_such_file_or_directory) return send(notFound(req.target()));

    if (ec) return send(serverError(ec.message()));

    const auto size = body.size();

    http::response<http::file_body> response{
        std::piecewise_construct, std::make_tuple(std::move(body)), std::make_tuple(http::status::ok, req.version())};
    response.set(http::field::server, XiboLocalWebServer);
    response.set(http::field::content_type, mimeType(path));
    response.content_length(size);
    response.keep_alive(req.keep_alive());
    return send(std::move(response));
}

Session::Session(tcp::socket&& socket, const FilePath& doc_root) :
    m_socket(std::move(socket)),
    m_rootDirectory(doc_root),
    m_lambda(*this)
{
}

void Session::run()
{
    doRead();
}

void Session::doRead()
{
    m_request = {};

    //    socket_.expires_after(std::chrono::seconds(30));

    http::async_read(m_socket, m_buffer, m_request, std::bind(&Session::onRead, shared_from_this(), ph::_1, ph::_2));
}

void Session::onRead(beast::error_code ec, std::size_t /*bytesTransferred*/)
{
    if (ec == http::error::end_of_stream) return close();

    if (!ec)
    {
        handleRequest(m_rootDirectory, std::move(m_request), m_lambda);
    }
    else
    {
        /* WebKitGTK sometimes resets connection but it's not a fatal error and we will reconnect */
        if (ec == net::error::connection_reset) return;

        Log::error("[WebServer] Read Error: {}", ec.message());
    }
}

void Session::onWrite(bool shouldBeClosed, beast::error_code ec, std::size_t /*bytesTransferred*/)
{
    if (!ec)
    {
        if (shouldBeClosed)
        {
            return close();
        }

        m_response = nullptr;
        doRead();
    }
    else
    {
        /* WebKitGTK sometimes resets connection but it's not a fatal error and we will reconnect */
        if (ec == net::error::connection_reset || ec == net::error::broken_pipe) return;

        Log::error("[WebServer] Write Error: {}", ec.message());
    }
}

void Session::close()
{
    beast::error_code ec;
    m_socket.shutdown(tcp::socket::shutdown_send, ec);
}

LocalWebServer::LocalWebServer() : work_(ioc_), acceptor_(ioc_)
{
    for (int i = 0; i != DefaultThreadsCount; ++i)
    {
        workerThreads_.push_back(std::make_unique<JoinableThread>([=]() {
            Log::trace("[WebServer] Thread started");

            ioc_.run();
        }));
    }
}

LocalWebServer::~LocalWebServer()
{
    ioc_.stop();
}

void LocalWebServer::run(unsigned short port)
{
    try
    {
        tcp::endpoint endpoint(net::ip::address::from_string(DefaultLocalAddress), port);
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(net::socket_base::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen(net::socket_base::max_listen_connections);
        port_ = port;

        doAccept();
    }
    catch (std::exception& e)
    {
        Log::error("[WebServer] Establish Error: {}", e.what());
    }
}

Uri LocalWebServer::address() const
{
    return Uri::fromString("http://localhost:" + std::to_string(port_) + "/");
}

void LocalWebServer::setRootDirectory(const FilePath& rootDirectory)
{
    rootDirectory_ = rootDirectory;
}

void LocalWebServer::doAccept()
{
    acceptor_.async_accept(ioc_, std::bind(&LocalWebServer::onAccept, shared_from_this(), ph::_1, ph::_2));
}

void LocalWebServer::onAccept(beast::error_code ec, tcp::socket socket)
{
    if (!ec)
    {
        std::make_shared<Session>(std::move(socket), rootDirectory_)->run();
    }
    else
    {
        Log::error("[WebSerber] Accept Connection Error: {}", ec.message());
    }

    doAccept();
}
