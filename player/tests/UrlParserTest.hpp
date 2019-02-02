#pragma once

#include "test_utils.hpp"
#include "managers/Uri.hpp"

struct UriTest
{
    std::string strUrl;
    Uri url;
};

class UriParserValidTests : public testing::TestWithParam<UriTest> { };
class UriParserInvalidTests : public testing::TestWithParam<UriTest> { };

const std::vector<UriTest> validUrls =
    {{"https://127.0.0.1/testtarget", Uri{Uri::Scheme::HTTPS, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 443}, "/testtarget"}},
     {"https://username@127.0.0.1/testtarget", Uri{Uri::Scheme::HTTPS, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 443}, "/testtarget"}},
     {"https://username@127.0.0.1:3434/testtarget", Uri{Uri::Scheme::HTTPS, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 3434}, "/testtarget"}},
     {"https://username:password@127.0.0.1/testtarget", Uri{Uri::Scheme::HTTPS, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 443}, "/testtarget"}},
     {"https://username:password@127.0.0.1:3434/testtarget", Uri{Uri::Scheme::HTTPS, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 3434}, "/testtarget"}},
     {"http://127.0.0.1/", Uri{Uri::Scheme::HTTP, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 80}, "/"}},
     {"https://127.0.0.1:1/testtarget", Uri{Uri::Scheme::HTTPS, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 1}, "/testtarget"}},
     {"https://127.0.0.1:12/testtarget", Uri{Uri::Scheme::HTTPS, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 12}, "/testtarget"}},
     {"https://127.0.0.1:123/testtarget", Uri{Uri::Scheme::HTTPS, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 123}, "/testtarget"}},
     {"https://127.0.0.1:1234/testtarget", Uri{Uri::Scheme::HTTPS, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 1234}, "/testtarget"}},
     {"https://127.0.0.1:12345/testtarget", Uri{Uri::Scheme::HTTPS, Uri::Authority{"127.0.0.1", Uri::Authority::HostType::IP, 12345}, "/testtarget"}},
     {"https://www.example.com/", Uri{Uri::Scheme::HTTPS, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 443}, "/"}},
     {"https://www.example.com:4343/", Uri{Uri::Scheme::HTTPS, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 4343}, "/"}},
     {"https://username@www.example.com/", Uri{Uri::Scheme::HTTPS, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 443}, "/"}},
     {"https://username@www.example.com:4343/", Uri{Uri::Scheme::HTTPS, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 4343}, "/"}},
     {"https://username:password@www.example.com/", Uri{Uri::Scheme::HTTPS, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 443}, "/"}},
     {"https://username:password@www.example.com:4343/", Uri{Uri::Scheme::HTTPS, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 4343}, "/"}},
     {"https://www.example.com/sub/", Uri{Uri::Scheme::HTTPS, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 443}, "/sub/"}},
     {"http://www.example.com/", Uri{Uri::Scheme::HTTP, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 80}, "/"}},
     {"http://blog.example.com/", Uri{Uri::Scheme::HTTP, Uri::Authority{"blog.example.com", Uri::Authority::HostType::DNS, 80}, "/"}},
     {"http://www.example.com/product", Uri{Uri::Scheme::HTTP, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 80}, "/product"}},
     {"http://www.example.com/products?id=1&page=2", Uri{Uri::Scheme::HTTP, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 80}, "/products?id=1&page=2"}},
     {"http://255.255.255.255/", Uri{Uri::Scheme::HTTP, Uri::Authority{"255.255.255.255", Uri::Authority::HostType::IP, 80}, "/"}},
     {"http://www.site.com:8008/", Uri{Uri::Scheme::HTTP, Uri::Authority{"www.site.com", Uri::Authority::HostType::DNS, 8008}, "/"}},
     {"file:///home/test/folder/", Uri{Uri::Scheme::File, Uri::Authority{}, "/home/test/folder/"}},
     {"https://john.doe@www.example.com:123/forum/questions/?tag=networking&order=newest#top", Uri{Uri::Scheme::HTTPS, Uri::Authority{"www.example.com", Uri::Authority::HostType::DNS, 123}, "/forum/questions/?tag=networking&order=newest#top"}},
     {"http://www.site.com/xibo:8008/", Uri{Uri::Scheme::HTTP, Uri::Authority{"www.site.com", Uri::Authority::HostType::DNS, 80}, "/xibo:8008/"}}};

const std::vector<UriTest> invalidUrls =
    {{"http://127.0.0.1:/", Uri{}},
     {"http://127.0.0.1:FDFDF/dfdf", Uri{}},
     {"https://127.0.0.1:123456/testtarget", Uri{}},
     {"127.0.0.1/testtarget:123456", Uri{}},
     {"www.example.com", Uri{}},
     {"example.com", Uri{}},
     {"http://www.example.com#up", Uri{}},
     {"255.255.255.255", Uri{}},
     {"http://www.site.com:8008", Uri{}}};
