#pragma once

#include "test_utils.hpp"
#include "managers/Url.hpp"

struct UrlTest
{
    std::string strUrl;
    Url url;
};

class UrlParserValidTests : public testing::TestWithParam<UrlTest> { };
class UrlParserInvalidTests : public testing::TestWithParam<UrlTest> { };

const std::vector<UrlTest> validUrls =
    {{"https://127.0.0.1/sadfsadf", Url{Url::Scheme::HTTPS, Url::Host{"127.0.0.1", Url::Host::Type::IP}, 443, "/sadfsadf"}},
     {"http://127.0.0.1/", Url{Url::Scheme::HTTP, Url::Host{"127.0.0.1", Url::Host::Type::IP}, 80, "/"}},
     {"https://127.0.0.1:1/sadfsadf", Url{Url::Scheme::HTTPS, Url::Host{"127.0.0.1", Url::Host::Type::IP}, 1, "/sadfsadf"}},
     {"https://127.0.0.1:12/sadfsadf", Url{Url::Scheme::HTTPS, Url::Host{"127.0.0.1", Url::Host::Type::IP}, 12, "/sadfsadf"}},
     {"https://127.0.0.1:123/sadfsadf", Url{Url::Scheme::HTTPS, Url::Host{"127.0.0.1", Url::Host::Type::IP}, 123, "/sadfsadf"}},
     {"https://127.0.0.1:1234/sadfsadf", Url{Url::Scheme::HTTPS, Url::Host{"127.0.0.1", Url::Host::Type::IP}, 1234, "/sadfsadf"}},
     {"https://127.0.0.1:12345/sadfsadf", Url{Url::Scheme::HTTPS, Url::Host{"127.0.0.1", Url::Host::Type::IP}, 12345, "/sadfsadf"}},
     {"https://www.example.com/", Url{Url::Scheme::HTTPS, Url::Host{"www.example.com", Url::Host::Type::DNS}, 443, "/"}},
     {"https://www.example.com/sub/", Url{Url::Scheme::HTTPS, Url::Host{"www.example.com/sub", Url::Host::Type::DNS}, 443, "/"}},
     {"http://www.example.com/", Url{Url::Scheme::HTTP, Url::Host{"www.example.com", Url::Host::Type::DNS}, 80, "/"}},
     {"http://blog.example.com/", Url{Url::Scheme::HTTP, Url::Host{"blog.example.com", Url::Host::Type::DNS}, 80, "/"}},
     {"http://www.example.com/product", Url{Url::Scheme::HTTP, Url::Host{"www.example.com", Url::Host::Type::DNS}, 80, "/product"}},
     {"http://www.example.com/products?id=1&page=2", Url{Url::Scheme::HTTP, Url::Host{"www.example.com", Url::Host::Type::DNS}, 80, "/products?id=1&page=2"}},
     {"http://255.255.255.255/", Url{Url::Scheme::HTTP, Url::Host{"255.255.255.255", Url::Host::Type::IP}, 80, "/"}},
     {"http://www.site.com:8008/", Url{Url::Scheme::HTTP, Url::Host{"www.site.com", Url::Host::Type::DNS}, 8008, "/"}},
     {"http://www.site.com/xibo:8008/", Url{Url::Scheme::HTTP, Url::Host{"www.site.com/xibo", Url::Host::Type::DNS}, 8008, "/"}}};

const std::vector<UrlTest> invalidUrls =
    {{"http://127.0.0.1:/", Url{}},
     {"file://127.0.0.1/", Url{}},
     {"http://127.0.0.1:FDFDF/dfdf", Url{}},
     {"https://127.0.0.1:123456/sadfsadf", Url{}},
     {"127.0.0.1/sadfsadf:123456", Url{}},
     {"www.example.com", Url{}},
     {"example.com", Url{}},
     {"http://www.example.com#up", Url{}},
     {"255.255.255.255", Url{}},
     {"http://www.site.com:8008", Url{}}};


const std::vector<std::string> invalidColors =
    {"#sss", "121", "asdfasdf", "11G", "#12H", "#111V",
    "#11", "#1", "1", "#dfdsaw", "#11111", "#1111111"
    "#111111111", "zzzzzz", "test me", "invalid",
    "#dfdfd1s", "#1111d", "", " ", "   "};
