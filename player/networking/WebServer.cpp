#include "WebServer.hpp"

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

    http::response<http::file_body> response{std::piecewise_construct, std::make_tuple(std::move(body)),
                                             std::make_tuple(http::status::ok, req.version())};
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
        Log::error("[WebServer] Write Error: {}", ec.message());
    }
}

void Session::close()
{
    beast::error_code ec;
    m_socket.shutdown(tcp::socket::shutdown_send, ec);
}

XiboWebServer::XiboWebServer() : m_work(m_ioc), m_acceptor(m_ioc)
{
    for (int i = 0; i != DefaultThreadsCount; ++i)
    {
        m_workerThreads.push_back(std::make_unique<JoinableThread>([=]() {
            Log::trace("[WebServer] Thread started");

            m_ioc.run();
        }));
    }
}

XiboWebServer::~XiboWebServer()
{
    m_ioc.stop();
}

std::string XiboWebServer::address() const
{
    return "http://" + DefaultLocalAddress + ":" + std::to_string(m_port) + "/";
}

void XiboWebServer::run(unsigned short port)
{
    try
    {
        tcp::endpoint endpoint(net::ip::address::from_string(DefaultLocalAddress), port);
        m_acceptor.open(endpoint.protocol());
        m_acceptor.set_option(net::socket_base::reuse_address(true));
        m_acceptor.bind(endpoint);
        m_acceptor.listen(net::socket_base::max_listen_connections);
        m_port = port;

        doAccept();
    }
    catch (std::exception& e)
    {
        Log::error("[WebServer] Establish Error: {}", e.what());
    }
}

void XiboWebServer::setRootDirectory(const FilePath& rootDirectory)
{
    m_rootDirectory = rootDirectory;
}

void XiboWebServer::doAccept()
{
    m_acceptor.async_accept(m_ioc, std::bind(&XiboWebServer::onAccept, shared_from_this(), ph::_1, ph::_2));
}

void XiboWebServer::onAccept(beast::error_code ec, tcp::socket socket)
{
    if (!ec)
    {
        std::make_shared<Session>(std::move(socket), m_rootDirectory)->run();
    }
    else
    {
        Log::error("[WebSerber] Accept Connection Error: {}", ec.message());
    }

    doAccept();
}
