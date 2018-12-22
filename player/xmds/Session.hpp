#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/core/flat_buffer.hpp>

template<typename Response, typename Request>
struct Session
{
    using callback = std::function<void(const boost::system::error_code&, const Response&)>;
    Session(boost::asio::io_context& ioc) : socket(ioc), resolver(ioc) { }
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver resolver;
    boost::beast::http::request<boost::beast::http::string_body> httpRequest;
    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::string_body> httpResponse;
    Request soapRequest;
    callback responseCallback;
};
