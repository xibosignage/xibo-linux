#include "BackgroundBuilderTests.hpp"

using namespace testing;

TEST(BackgroundBuilderTest, Construct_WithoutPathAndColor_ShouldThrowRunTimeError)
{
    ASSERT_THROW(BackgroundBuilderTest().defaultSize().build(), std::runtime_error);
}

TEST_P(BackgroundBuilderSizeTest, Construct_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(BackgroundBuilderTest().width(GetParam().width).height(GetParam().height).build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundBuilderSizeTest, ::testing::ValuesIn(invalidBackgroundSizes));

TEST_P(BackgroundBuilderValidColorTest, Construct_ValidColor_HandlerSetColorWithConvertedStringShouldBeCalled)
{
    auto background = BackgroundBuilderTest().defaultSize().color(GetParam().strColor).build();
    ASSERT_EQ(static_cast<MockOneColorBackground*>(background.get())->color(), GetParam().numberColor);
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundBuilderValidColorTest, ::testing::ValuesIn(validColors));

TEST_P(BackgroundBuilderInvalidColorTest, Construct_InvalidColor_InvalidArgumentShouldBeThrown)
{
    ASSERT_THROW(BackgroundBuilderTest().defaultSize().color(GetParam()).build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundBuilderInvalidColorTest, ::testing::ValuesIn(invalidColors));

TEST(BackgroundBuilderTest, Construct_DefaultColorPath_ColorShouldNotBeSetPathShouldBeSet)
{
    BackgroundBuilderTest builder;

    ON_CALL(builder.filesystem(), isRegularFile(_)).WillByDefault(Return(true));

    auto background = builder.defaultSize().color(DEFAULT_COLOR).path(DEFAULT_PATH.string()).build();
    EXPECT_TRUE(dynamic_cast<MockImageBackground*>(background.get()));
}

TEST(BackgroundBuilderTest, Construct_InvalidPath_ShouldThrowRunTimeError)
{
    BackgroundBuilderTest builder;

    ON_CALL(builder.filesystem(), isRegularFile(_)).WillByDefault(Return(false));

    ASSERT_THROW(builder.defaultSize().path(DEFAULT_PATH.string()).build(), std::runtime_error);
}

TEST(BackgroundBuilderTest, Construct_ValidPath_PathShouldBeSetCorrectly)
{
    BackgroundBuilderTest builder;
    FilePath fullPath(DEFAULT_RESOURCES_DIR / DEFAULT_PATH);

    ON_CALL(builder.filesystem(), isRegularFile(fullPath)).WillByDefault(Return(true));

    auto background = builder.defaultSize().path(DEFAULT_PATH.string()).build();
    ASSERT_EQ(static_cast<MockImageBackground*>(background.get())->path(), fullPath);
}

