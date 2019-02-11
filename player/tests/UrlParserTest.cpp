#include "UrlParserTest.hpp"

#include "managers/UrlParser.hpp"

TEST_P(UrlParserValidTests, UrlParser_Parse_ValidUrl)
{
    UrlParser parser;
    auto urlToTest = parser.parse(GetParam().strUrl);
    auto comparedUrl = GetParam().url;

    ASSERT_EQ(urlToTest.scheme, comparedUrl.scheme);
    ASSERT_EQ(urlToTest.host.host, comparedUrl.host.host);
    ASSERT_EQ(urlToTest.host.type, comparedUrl.host.type);
    ASSERT_EQ(urlToTest.port, comparedUrl.port);
    ASSERT_EQ(urlToTest.target, comparedUrl.target);
}

INSTANTIATE_TEST_CASE_P(Suite, UrlParserValidTests, ::testing::ValuesIn(validUrls));

TEST_P(UrlParserInvalidTests, UrlParser_Parse_InvalidUrl)
{
    UrlParser parser;

    ASSERT_THROW(parser.parse(GetParam().strUrl), std::runtime_error);
}

INSTANTIATE_TEST_CASE_P(Suite, UrlParserInvalidTests, ::testing::ValuesIn(invalidUrls));
