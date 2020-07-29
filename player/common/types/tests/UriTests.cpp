#include "UriTests.hpp"

#include "common/fs/FilePath.hpp"
#include "common/types/internal/UriParser.hpp"

TEST_P(UriValidHttpTests, FromString_ValidNetworkScheme)
{
    auto uri = Uri::fromString(GetParam().source);

    ASSERT_EQ(static_cast<std::string>(uri.scheme()), GetParam().scheme);
    ASSERT_TRUE(uri.optionalAuthority());
    ASSERT_EQ(uri.authority().host(), GetParam().host);
    ASSERT_EQ(uri.authority().optionalPort(), GetParam().port);
    ASSERT_EQ(uri.path(), GetParam().path);
    ASSERT_EQ(uri.string(), GetParam().str);
}

INSTANTIATE_TEST_CASE_P(Suite, UriValidHttpTests, ::testing::ValuesIn(validUris));

TEST_P(UriValidWithoutAuthorityTests, FromString)
{
    auto uri = Uri::fromString(GetParam().source);

    ASSERT_EQ(static_cast<std::string>(uri.scheme()), GetParam().scheme);
    ASSERT_FALSE(uri.optionalAuthority());
    ASSERT_EQ(uri.path(), GetParam().path);
    ASSERT_EQ(uri.string(), GetParam().str);
}

INSTANTIATE_TEST_CASE_P(Suite, UriValidWithoutAuthorityTests, ::testing::ValuesIn(validUrisWithoutAuhority));

TEST_P(UriInvalidTests, FromString_Invalid)
{
    ASSERT_THROW(Uri::fromString(GetParam()), UriParser::Error);
}

INSTANTIATE_TEST_CASE_P(Suite, UriInvalidTests, ::testing::ValuesIn(invalidUrls));

TEST(UriManualInit, FromFile)
{
    auto uri = Uri::fromFile("/my/path");

    ASSERT_EQ(static_cast<std::string>(uri.scheme()), "file");
    ASSERT_EQ(uri.path(), "/my/path");
    ASSERT_FALSE(uri.optionalAuthority().has_value());
    ASSERT_EQ(uri.string(), "file:///my/path");
}

TEST(UriManualInit, SchemeHostAndPath)
{
    Uri uri{Uri::Scheme{"http"}, Uri::Host::fromString("test.com"), "/target"};

    ASSERT_EQ(static_cast<std::string>(uri.scheme()), "http");
    ASSERT_EQ(uri.path(), "/target");
    ASSERT_EQ(uri.optionalAuthority()->host(), (HostTest{"test.com", Uri::Host::Type::DNS}));
    ASSERT_EQ(static_cast<unsigned short>(uri.authority().port()), 80);
    ASSERT_EQ(uri.string(), "http://test.com/target");
    ASSERT_FALSE(uri.authority().optionalUserInfo().has_value());
}

TEST(UriManualInit, SchemeAuthorityAndPath)
{
    Uri uri{Uri::Scheme{"http"},
            Uri::Authority{Uri::UserInfo{"username", "password"}, Uri::Host::fromString("test.com"), Uri::Port{1234}},
            "/target"};

    ASSERT_EQ(static_cast<std::string>(uri.scheme()), "http");
    ASSERT_EQ(uri.path(), "/target");
    ASSERT_EQ(uri.authority().host(), (HostTest{"test.com", Uri::Host::Type::DNS}));
    ASSERT_EQ(static_cast<unsigned short>(uri.authority().port()), 1234);
    ASSERT_EQ(uri.string(), "http://username:password@test.com:1234/target");
    ASSERT_EQ(static_cast<std::string>(uri.authority().userInfo()), std::string{"username:password"});
}
