#include "UrlParserTest.hpp"

#include "managers/UriParser.hpp"

TEST_P(UriParserValidTests, UriParser_Parse_ValidUrl)
{
    UriParser parser;
    auto urlToTest = parser.parse(GetParam().strUrl);
    auto comparedUrl = GetParam().url;

    ASSERT_EQ(urlToTest->scheme, comparedUrl.scheme);
    ASSERT_EQ(urlToTest->authority.host, comparedUrl.authority.host);
    ASSERT_EQ(urlToTest->authority.hostType, comparedUrl.authority.hostType);
    ASSERT_EQ(urlToTest->authority.port, comparedUrl.authority.port);
    ASSERT_EQ(urlToTest->path, comparedUrl.path);
}

INSTANTIATE_TEST_CASE_P(Suite, UriParserValidTests, ::testing::ValuesIn(validUrls));

TEST_P(UriParserInvalidTests, UriParser_Parse_InvalidUrl)
{
    UriParser parser;
    auto urlToTest = parser.parse(GetParam().strUrl);

    ASSERT_FALSE(urlToTest.has_value());
}

INSTANTIATE_TEST_CASE_P(Suite, UriParserInvalidTests, ::testing::ValuesIn(invalidUrls));
