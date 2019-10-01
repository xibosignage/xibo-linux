#include "UriTests.hpp"

#include "common/fs/FilePath.hpp"
#include "common/types/UriParser.hpp"

TEST_P(UriValidHttpTests, InitializeWithValidHttpUrl)
{
    auto uri = Uri::fromString(GetParam().source);

    ASSERT_EQ(uri.scheme(), GetParam().scheme);
    ASSERT_EQ(uri.host(), GetParam().host);
    ASSERT_EQ(uri.hostType(), GetParam().hostType);
    ASSERT_EQ(uri.port(), GetParam().port.value());
    ASSERT_EQ(uri.path(), GetParam().path);
    ASSERT_EQ(uri.string(), GetParam().str);
}

INSTANTIATE_TEST_CASE_P(Suite, UriValidHttpTests, ::testing::ValuesIn(validHttpUris));

TEST_P(UriValidFileTests, InitializeWithValidPath)
{
    auto uri = Uri::fromString(GetParam().source);

    ASSERT_EQ(uri.scheme(), GetParam().scheme);
    ASSERT_EQ(uri.host(), GetParam().host);
    ASSERT_EQ(uri.hostType(), GetParam().hostType);
    ASSERT_EQ(uri.path(), GetParam().path);
    ASSERT_EQ(uri.string(), GetParam().str);
}

INSTANTIATE_TEST_CASE_P(Suite, UriValidFileTests, ::testing::ValuesIn(validFileUris));

TEST_P(UriInvalidTests, InitializeWithInvalidString)
{
    ASSERT_THROW(Uri::fromString(GetParam()), UriParser::Error);
}

INSTANTIATE_TEST_CASE_P(Suite, UriInvalidTests, ::testing::ValuesIn(invalidUrls));

TEST(UriManualInit, SchemeAndPath)
{
    Uri uri{"/my/path"};

    ASSERT_EQ(uri.scheme(), Uri::Scheme::File);
    ASSERT_EQ(uri.path(), "/my/path");
    ASSERT_EQ(uri.host(), std::string{});
    ASSERT_EQ(uri.hostType(), Uri::Authority::HostType::Invalid);
    ASSERT_EQ(uri.string(), "file:///my/path");
    ASSERT_FALSE(uri.credentials().has_value());
}

TEST(UriManualInit, SchemeHostAndPath)
{
    Uri uri{Uri::Scheme::HTTP, "test.com", "/target"};

    ASSERT_EQ(uri.scheme(), Uri::Scheme::HTTP);
    ASSERT_EQ(uri.path(), "/target");
    ASSERT_EQ(uri.host(), "test.com");
    ASSERT_EQ(uri.port(), 80);
    ASSERT_EQ(uri.hostType(), Uri::Authority::HostType::DNS);
    ASSERT_EQ(uri.string(), "http://test.com/target");
    ASSERT_FALSE(uri.credentials().has_value());
}

TEST(UriManualInit, SchemeAuthorityAndPath)
{
    Uri uri{Uri::Scheme::HTTP, Uri::Authority{std::string{"username:password"}, "test.com", 1234}, "/target"};

    ASSERT_EQ(uri.scheme(), Uri::Scheme::HTTP);
    ASSERT_EQ(uri.path(), "/target");
    ASSERT_EQ(uri.host(), "test.com");
    ASSERT_EQ(uri.port(), 1234);
    ASSERT_EQ(uri.hostType(), Uri::Authority::HostType::DNS);
    ASSERT_EQ(uri.string(), "http://username:password@test.com:1234/target");
    ASSERT_EQ(uri.credentials(), std::string{"username:password"});
}
