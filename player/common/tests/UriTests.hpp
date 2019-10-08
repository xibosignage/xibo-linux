#pragma once

#include "common/types/Uri.hpp"
#include <gtest/gtest.h>

struct HostTest
{
    std::string str;
    Uri::Host::Type type;
};

struct UriTest
{
    UriTest(std::string source_,
            std::string scheme_,
            boost::optional<HostTest> host_,
            boost::optional<unsigned short> port_,
            std::string path_,
            std::string str_ = {}) :
        source(source_),
        scheme(scheme_),
        host(host_),
        port(port_),
        path(path_),
        str(str_.empty() ? source_ : str_)
    {
    }

    std::string source;
    std::string scheme;
    boost::optional<HostTest> host;
    boost::optional<unsigned short> port;
    std::string path;
    std::string str;
};

bool operator==(const boost::optional<Uri::Host>& host, const boost::optional<HostTest>& hostTest)
{
    if (!host.has_value() && !hostTest.has_value()) return true;

    if (host.has_value() && hostTest.has_value())
    {
        return static_cast<std::string>(host.value()) == hostTest->str && host->type() == hostTest->type;
    }

    return false;
}

bool operator==(const boost::optional<Uri::Port>& port, const boost::optional<unsigned short>& portTest)
{
    if (!port.has_value() && !portTest.has_value()) return true;

    if (port.has_value() && portTest.has_value())
    {
        return static_cast<unsigned short>(port.value()) == portTest.value();
    }

    return false;
}
class UriValidHttpTests : public testing::TestWithParam<UriTest>
{
};
class UriValidWithoutAuthorityTests : public testing::TestWithParam<UriTest>
{
};
class UriInvalidTests : public testing::TestWithParam<std::string>
{
};

const std::vector<UriTest> validUris = {
    {"https://127.0.0.1/testtarget", "https", HostTest{"127.0.0.1", Uri::Host::Type::IP}, 443, "/testtarget"},
    {"https://username@127.0.0.1/testtarget", "https", HostTest{"127.0.0.1", Uri::Host::Type::IP}, 443, "/testtarget"},
    {"https://username@127.0.0.1:1234/testtarget",
     "https",
     HostTest{"127.0.0.1", Uri::Host::Type::IP},
     1234,
     "/testtarget"},
    {"https://username:password@127.0.0.1/testtarget",
     "https",
     HostTest{"127.0.0.1", Uri::Host::Type::IP},
     443,
     "/testtarget"},
    {"https://username:password@127.0.0.1:1234/testtarget",
     "https",
     HostTest{"127.0.0.1", Uri::Host::Type::IP},
     1234,
     "/testtarget"},
    {"http://127.0.0.1/", "http", HostTest{"127.0.0.1", Uri::Host::Type::IP}, 80, "/"},
    {"https://127.0.0.1:1/testtarget", "https", HostTest{"127.0.0.1", Uri::Host::Type::IP}, 1, "/testtarget"},
    {"https://127.0.0.1:12/testtarget", "https", HostTest{"127.0.0.1", Uri::Host::Type::IP}, 12, "/testtarget"},
    {"https://127.0.0.1:123/testtarget", "https", HostTest{"127.0.0.1", Uri::Host::Type::IP}, 123, "/testtarget"},
    {"https://127.0.0.1:1234/testtarget", "https", HostTest{"127.0.0.1", Uri::Host::Type::IP}, 1234, "/testtarget"},
    {"https://127.0.0.1:12345/testtarget", "https", HostTest{"127.0.0.1", Uri::Host::Type::IP}, 12345, "/testtarget"},
    {"https://www.example.com/", "https", HostTest{"www.example.com", Uri::Host::Type::DNS}, 443, "/"},
    {"https://www.example.com:1234/", "https", HostTest{"www.example.com", Uri::Host::Type::DNS}, 1234, "/"},
    {"https://username@www.example.com/", "https", HostTest{"www.example.com", Uri::Host::Type::DNS}, 443, "/"},
    {"https://username@www.example.com:1234/", "https", HostTest{"www.example.com", Uri::Host::Type::DNS}, 1234, "/"},
    {"https://username:password@www.example.com/",
     "https",
     HostTest{"www.example.com", Uri::Host::Type::DNS},
     443,
     "/"},
    {"https://username:password@www.example.com:1234/",
     "https",
     HostTest{"www.example.com", Uri::Host::Type::DNS},
     1234,
     "/"},
    {"https://www.example.com/sub/", "https", HostTest{"www.example.com", Uri::Host::Type::DNS}, 443, "/sub/"},
    {"http://www.example.com/", "http", HostTest{"www.example.com", Uri::Host::Type::DNS}, 80, "/"},
    {"http://blog.example.com/", "http", HostTest{"blog.example.com", Uri::Host::Type::DNS}, 80, "/"},
    {"http://www.example.com/product", "http", HostTest{"www.example.com", Uri::Host::Type::DNS}, 80, "/product"},
    {"http://www.example.com/products?id=1&page=2",
     "http",
     HostTest{"www.example.com", Uri::Host::Type::DNS},
     80,
     "/products?id=1&page=2"},
    {"http://255.255.255.255/", "http", HostTest{"255.255.255.255", Uri::Host::Type::IP}, 80, "/"},
    {"http://www.site.com:1234/", "http", HostTest{"www.site.com", Uri::Host::Type::DNS}, 1234, "/"},
    {"http://www.site.com/xibo:1234/", "http", HostTest{"www.site.com", Uri::Host::Type::DNS}, 80, "/xibo:1234/"},
    {"http://www.site.com:1234",
     "http",
     HostTest{"www.site.com", Uri::Host::Type::DNS},
     1234,
     "/",
     "http://www.site.com:1234/"},
    {"http://www.example.com#up",
     "http",
     HostTest{"www.example.com", Uri::Host::Type::DNS},
     80,
     "/#up",
     "http://www.example.com/#up"},
    {"https://xibo.org.uk?p=help&q=what",
     "https",
     HostTest{"xibo.org.uk", Uri::Host::Type::DNS},
     443,
     "/?p=help&q=what",
     "https://xibo.org.uk/?p=help&q=what"},
    {"http://127.0.0.1?query",
     "http",
     HostTest{"127.0.0.1", Uri::Host::Type::IP},
     80,
     "/?query",
     "http://127.0.0.1/?query"},
    {"https://xibo.org.uk?p=help&q=what#fragment",
     "https",
     HostTest{"xibo.org.uk", Uri::Host::Type::DNS},
     443,
     "/?p=help&q=what#fragment",
     "https://xibo.org.uk/?p=help&q=what#fragment"},
    {"http://www.example.com",
     "http",
     HostTest{"www.example.com", Uri::Host::Type::DNS},
     80,
     "/",
     "http://www.example.com/"},
    {"http://127.0.0.1", "http", HostTest{"127.0.0.1", Uri::Host::Type::IP}, 80, "/", "http://127.0.0.1/"},
    {"http://127.0.0.1:1234", "http", HostTest{"127.0.0.1", Uri::Host::Type::IP}, 1234, "/", "http://127.0.0.1:1234/"},
    {"https://john.doe@www.example.com:123/forum/questions/?tag=networking&order=newest#top",
     "https",
     HostTest{"www.example.com", Uri::Host::Type::DNS},
     123,
     "/forum/questions/?tag=networking&order=newest#top"},
    {"file://www.example.com/home/test/folder/",
     "file",
     HostTest{"www.example.com", Uri::Host::Type::DNS},
     {},
     "/home/test/folder/"},
    {"file://127.0.0.1/home/test/folder/",
     "file",
     HostTest{"127.0.0.1", Uri::Host::Type::IP},
     {},
     "/home/test/folder/"}};

const std::vector<UriTest> validUrisWithoutAuhority = {
    {"file:///home/test/folder/", "file", {}, {}, "/home/test/folder/"},
    {"file:///home/test/folder/file.html", "file", {}, {}, "/home/test/folder/file.html"}};

const std::vector<std::string> invalidUrls = {{"127.0.0.1"},                            // no scheme
                                              {"127.0.0.1:1234"},                       // no scheme
                                              {"127.0.0.1/"},                           // no scheme
                                              {"127.0.0.1:1234/"},                      // no scheme
                                              {"http://127.0.0.1:/"},                   // no port but with :
                                              {"http://127.0.0.1:"},                    // no port but with :
                                              {"http://127.0.0.1:FDFDF/dfdf"},          // wrong port format
                                              {"https://127.0.0.1:123456/testtarget"},  // wrong port format
                                              {"127.0.0.1/testtarget:123456"},          // no scheme
                                              {"www.example.com"},                      // no scheme
                                              {"example.com"},                          // no scheme
                                              {"255.255.255.255"}};                     // no scheme
