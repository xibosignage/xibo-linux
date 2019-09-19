#pragma once

#include "uri/Uri.hpp"
#include <gtest/gtest.h>

struct UriTest
{
    UriTest(std::string source_,
            Uri::Scheme scheme_,
            std::string host_,
            unsigned short port_,
            std::string path_,
            Uri::Authority::HostType hostType_  = Uri::Authority::HostType::Invalid,
            std::string str_ = {}) :
        source(source_),
        scheme(scheme_),
        host(host_),
        port(port_),
        path(path_),
        hostType(hostType_),
        str(str_.empty() ? source_ : str_)
    {
    }

    std::string source;
    Uri::Scheme scheme;
    std::string host;
    boost::optional<unsigned short> port;
    std::string path;
    Uri::Authority::HostType hostType = Uri::Authority::HostType::Invalid;
    std::string str;
};

class UriValidHttpTests : public testing::TestWithParam<UriTest> { };
class UriValidFileTests : public testing::TestWithParam<UriTest> { };
class UriInvalidTests : public testing::TestWithParam<std::string> { };

const std::vector<UriTest> validHttpUris =
    {{"https://127.0.0.1/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 443, "/testtarget", Uri::Authority::HostType::IP},
     {"https://username@127.0.0.1/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 443, "/testtarget", Uri::Authority::HostType::IP},
     {"https://username@127.0.0.1:1234/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 1234, "/testtarget", Uri::Authority::HostType::IP},
     {"https://username:password@127.0.0.1/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 443, "/testtarget", Uri::Authority::HostType::IP},
     {"https://username:password@127.0.0.1:1234/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 1234, "/testtarget", Uri::Authority::HostType::IP},
     {"http://127.0.0.1/", Uri::Scheme::HTTP, "127.0.0.1", 80, "/", Uri::Authority::HostType::IP},
     {"https://127.0.0.1:1/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 1, "/testtarget", Uri::Authority::HostType::IP},
     {"https://127.0.0.1:12/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 12, "/testtarget", Uri::Authority::HostType::IP},
     {"https://127.0.0.1:123/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 123, "/testtarget", Uri::Authority::HostType::IP},
     {"https://127.0.0.1:1234/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 1234, "/testtarget", Uri::Authority::HostType::IP},
     {"https://127.0.0.1:12345/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 12345, "/testtarget", Uri::Authority::HostType::IP},
     {"https://www.example.com/", Uri::Scheme::HTTPS, "www.example.com", 443, "/", Uri::Authority::HostType::DNS},
     {"https://www.example.com:1234/", Uri::Scheme::HTTPS, "www.example.com", 1234, "/", Uri::Authority::HostType::DNS},
     {"https://username@www.example.com/", Uri::Scheme::HTTPS, "www.example.com", 443, "/", Uri::Authority::HostType::DNS},
     {"https://username@www.example.com:1234/", Uri::Scheme::HTTPS, "www.example.com", 1234, "/", Uri::Authority::HostType::DNS},
     {"https://username:password@www.example.com/", Uri::Scheme::HTTPS, "www.example.com", 443, "/", Uri::Authority::HostType::DNS},
     {"https://username:password@www.example.com:1234/", Uri::Scheme::HTTPS, "www.example.com", 1234, "/", Uri::Authority::HostType::DNS},
     {"https://www.example.com/sub/", Uri::Scheme::HTTPS, "www.example.com", 443, "/sub/", Uri::Authority::HostType::DNS},
     {"http://www.example.com/", Uri::Scheme::HTTP, "www.example.com", 80, "/", Uri::Authority::HostType::DNS},
     {"http://blog.example.com/", Uri::Scheme::HTTP, "blog.example.com", 80, "/", Uri::Authority::HostType::DNS},
     {"http://www.example.com/product", Uri::Scheme::HTTP, "www.example.com", 80, "/product", Uri::Authority::HostType::DNS},
     {"http://www.example.com/products?id=1&page=2", Uri::Scheme::HTTP, "www.example.com", 80, "/products?id=1&page=2", Uri::Authority::HostType::DNS},
     {"http://255.255.255.255/", Uri::Scheme::HTTP, "255.255.255.255", 80, "/", Uri::Authority::HostType::IP},
     {"http://www.site.com:1234/", Uri::Scheme::HTTP, "www.site.com", 1234, "/", Uri::Authority::HostType::DNS},
     {"http://www.site.com/xibo:1234/", Uri::Scheme::HTTP, "www.site.com", 80, "/xibo:1234/", Uri::Authority::HostType::DNS},
     {"http://www.site.com:1234", Uri::Scheme::HTTP, "www.site.com", 1234, "/", Uri::Authority::HostType::DNS, "http://www.site.com:1234/"},
     {"http://www.example.com#up", Uri::Scheme::HTTP, "www.example.com", 80, "/#up", Uri::Authority::HostType::DNS, "http://www.example.com/#up"},
     {"https://xibo.org.uk?p=help&q=what", Uri::Scheme::HTTPS, "xibo.org.uk", 443, "/?p=help&q=what", Uri::Authority::HostType::DNS, "https://xibo.org.uk/?p=help&q=what"},
     {"http://127.0.0.1?query", Uri::Scheme::HTTP, "127.0.0.1", 80, "/?query", Uri::Authority::HostType::IP, "http://127.0.0.1/?query"},
     {"https://xibo.org.uk?p=help&q=what#fragment", Uri::Scheme::HTTPS, "xibo.org.uk", 443, "/?p=help&q=what#fragment", Uri::Authority::HostType::DNS, "https://xibo.org.uk/?p=help&q=what#fragment"},
     {"http://www.example.com", Uri::Scheme::HTTP, "www.example.com", 80, "/", Uri::Authority::HostType::DNS, "http://www.example.com/"},
     {"http://127.0.0.1", Uri::Scheme::HTTP, "127.0.0.1", 80, "/", Uri::Authority::HostType::IP, "http://127.0.0.1/"},
     {"http://127.0.0.1:1234", Uri::Scheme::HTTP, "127.0.0.1", 1234, "/", Uri::Authority::HostType::IP, "http://127.0.0.1:1234/"},
     {"https://john.doe@www.example.com:123/forum/questions/?tag=networking&order=newest#top", Uri::Scheme::HTTPS, "www.example.com", 123, "/forum/questions/?tag=networking&order=newest#top", Uri::Authority::HostType::DNS}
    };


const std::vector<UriTest> validFileUris =
    {{"file://www.example.com/home/test/folder/", Uri::Scheme::FILE, "www.example.com", {}, "/home/test/folder/", Uri::Authority::HostType::DNS},
     {"file://127.0.0.1/home/test/folder/", Uri::Scheme::FILE, "127.0.0.1", {}, "/home/test/folder/", Uri::Authority::HostType::IP},
     {"file:///home/test/folder/", Uri::Scheme::FILE, {}, {}, "/home/test/folder/"},
     {"file:///home/test/folder/file.html", Uri::Scheme::FILE, {}, {}, "/home/test/folder/file.html"}};

const std::vector<std::string> invalidUrls =
    {{"127.0.0.1"}, // no scheme
     {"127.0.0.1:1234"}, // no scheme
     {"127.0.0.1/"}, // no scheme
     {"127.0.0.1:1234/"}, // no scheme
     {"http://127.0.0.1:/"}, // no port but with :
     {"http://127.0.0.1:"}, // no port but with :
     {"http://127.0.0.1:FDFDF/dfdf"}, // wrong port format
     {"https://127.0.0.1:123456/testtarget"}, // wrong port format
     {"127.0.0.1/testtarget:123456"}, // no scheme
     {"www.example.com"}, // no scheme
     {"example.com"}, // no scheme
     {"255.255.255.255"}}; // no scheme
