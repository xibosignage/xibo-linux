#pragma once

#include "test_utils.hpp"
#include "managers/Uri.hpp"

struct UriTest
{
    std::string strUrl;
    Uri::Scheme scheme;
    std::string host;
    boost::optional<unsigned short> port;
    std::string path;
    Uri::Authority::HostType hostType = Uri::Authority::HostType::Invalid;
};

class UriParserValidHttpTests : public testing::TestWithParam<UriTest> { };
class UriParserValidFileTests : public testing::TestWithParam<UriTest> { };
class UriParserInvalidTests : public testing::TestWithParam<std::string> { };

const std::vector<UriTest> validHttpUris =
    {{"https://127.0.0.1/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 443, "/testtarget", Uri::Authority::HostType::IP},
     {"https://username@127.0.0.1/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 443, "/testtarget", Uri::Authority::HostType::IP},
     {"https://username@127.0.0.1:3434/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 3434, "/testtarget", Uri::Authority::HostType::IP},
     {"https://username:password@127.0.0.1/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 443, "/testtarget", Uri::Authority::HostType::IP},
     {"https://username:password@127.0.0.1:3434/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 3434, "/testtarget", Uri::Authority::HostType::IP},
     {"http://127.0.0.1/", Uri::Scheme::HTTP, "127.0.0.1", 80, "/", Uri::Authority::HostType::IP},
     {"https://127.0.0.1:1/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 1, "/testtarget", Uri::Authority::HostType::IP},
     {"https://127.0.0.1:12/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 12, "/testtarget", Uri::Authority::HostType::IP},
     {"https://127.0.0.1:123/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 123, "/testtarget", Uri::Authority::HostType::IP},
     {"https://127.0.0.1:1234/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 1234, "/testtarget", Uri::Authority::HostType::IP},
     {"https://127.0.0.1:12345/testtarget", Uri::Scheme::HTTPS, "127.0.0.1", 12345, "/testtarget", Uri::Authority::HostType::IP},
     {"https://www.example.com/", Uri::Scheme::HTTPS, "www.example.com", 443, "/", Uri::Authority::HostType::DNS},
     {"https://www.example.com:4343/", Uri::Scheme::HTTPS, "www.example.com", 4343, "/", Uri::Authority::HostType::DNS},
     {"https://username@www.example.com/", Uri::Scheme::HTTPS, "www.example.com", 443, "/", Uri::Authority::HostType::DNS},
     {"https://username@www.example.com:4343/", Uri::Scheme::HTTPS, "www.example.com", 4343, "/", Uri::Authority::HostType::DNS},
     {"https://username:password@www.example.com/", Uri::Scheme::HTTPS, "www.example.com", 443, "/", Uri::Authority::HostType::DNS},
     {"https://username:password@www.example.com:4343/", Uri::Scheme::HTTPS, "www.example.com", 4343, "/", Uri::Authority::HostType::DNS},
     {"https://www.example.com/sub/", Uri::Scheme::HTTPS, "www.example.com", 443, "/sub/", Uri::Authority::HostType::DNS},
     {"http://www.example.com/", Uri::Scheme::HTTP, "www.example.com", 80, "/", Uri::Authority::HostType::DNS},
     {"http://blog.example.com/", Uri::Scheme::HTTP, "blog.example.com", 80, "/", Uri::Authority::HostType::DNS},
     {"http://www.example.com/product", Uri::Scheme::HTTP, "www.example.com", 80, "/product", Uri::Authority::HostType::DNS},
     {"http://www.example.com/products?id=1&page=2", Uri::Scheme::HTTP, "www.example.com", 80, "/products?id=1&page=2", Uri::Authority::HostType::DNS},
     {"http://255.255.255.255/", Uri::Scheme::HTTP, "255.255.255.255", 80, "/", Uri::Authority::HostType::IP},
     {"http://www.site.com:8008/", Uri::Scheme::HTTP, "www.site.com", 8008, "/", Uri::Authority::HostType::DNS},
     {"https://john.doe@www.example.com:123/forum/questions/?tag=networking&order=newest#top", Uri::Scheme::HTTPS, "www.example.com", 123, "/forum/questions/?tag=networking&order=newest#top", Uri::Authority::HostType::DNS},
     {"http://www.site.com/xibo:8008/", Uri::Scheme::HTTP, "www.site.com", 80, "/xibo:8008/", Uri::Authority::HostType::DNS}};


const std::vector<UriTest> validFileUris =
    {{"file://www.example.com/home/test/folder/", Uri::Scheme::File, "www.example.com", {}, "/home/test/folder/", Uri::Authority::HostType::DNS},
     {"file://127.0.0.1/home/test/folder/", Uri::Scheme::File, "127.0.0.1", {}, "/home/test/folder/", Uri::Authority::HostType::IP},
     {"file:///home/test/folder/", Uri::Scheme::File, {}, {}, "/home/test/folder/"},
     {"file:///home/test/folder/file.html", Uri::Scheme::File, {}, {}, "/home/test/folder/file.html"}};

const std::vector<std::string> invalidUrls =
    {{"http://127.0.0.1:/"},
     {"http://127.0.0.1:FDFDF/dfdf"},
     {"https://127.0.0.1:123456/testtarget"},
     {"127.0.0.1/testtarget:123456"},
     {"www.example.com"},
     {"example.com"},
     {"http://www.example.com#up"},
     {"255.255.255.255"},
     {"http://www.site.com:8008"}};
