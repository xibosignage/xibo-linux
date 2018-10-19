#include "BackgroundBuilderTests.hpp"

using namespace testing;
using namespace std::string_literals;

TEST(BackgroundBuilderTest, Construct_ValidSize_HandlerSetSizeShouldBeCalled)
{
    auto backgroundHandlerMock = std::make_unique<NiceMock<MockImageAdaptor>>();

    EXPECT_CALL(*backgroundHandlerMock, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    BackgroundBuilderTest().defaultSize().adaptor(std::move(backgroundHandlerMock)).build();
}

TEST_P(BackgroundBuilderSizeTest, Construct_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(BackgroundBuilderTest().width(GetParam().width).height(GetParam().height).build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundBuilderSizeTest, ::testing::ValuesIn(invalidBackgroundSizes));

TEST_P(BackgroundBuilderValidColorTest, Construct_ValidColor_HandlerSetColorWithConvertedStringShouldBeCalled)
{
    auto backgroundHandlerMock = std::make_unique<NiceMock<MockImageAdaptor>>();

    EXPECT_CALL(*backgroundHandlerMock, setColor(GetParam().numberColor));

    BackgroundBuilderTest().defaultSize().adaptor(std::move(backgroundHandlerMock)).color(GetParam().strColor).build();
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundBuilderValidColorTest, ::testing::ValuesIn(validColors));

TEST_P(BackgroundBuilderInvalidColorTest, Construct_InvalidColor_InvalidArgumentShouldBeThrown)
{
    ASSERT_THROW(BackgroundBuilderTest().defaultAdaptor().defaultSize().color(GetParam()).build(), std::invalid_argument);
}

TEST_P(BackgroundBuilderInvalidColorTest, Construct_InvalidColor_HandlerSetColorShouldNotBeCalled)
{
    auto backgroundHandlerMock = std::make_unique<NiceMock<MockImageAdaptor>>();

    EXPECT_CALL(*backgroundHandlerMock, setColor(_)).Times(0);

    EXPECT_ANY_THROW(BackgroundBuilderTest().defaultSize().adaptor(std::move(backgroundHandlerMock)).color(GetParam()).build());
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundBuilderInvalidColorTest, ::testing::ValuesIn(invalidColors));

TEST(BackgroundBuilderTest, Construct_DefaultColorPath_HandlerSetColorShouldNotBeCalled)
{
    auto adaptor = std::make_unique<NiceMock<MockFileSystemAdaptor>>();
    auto backgroundHandlerMock = std::make_unique<NiceMock<MockImageAdaptor>>();

    ON_CALL(*adaptor, isRegularFile(_)).WillByDefault(Return(true));
    EXPECT_CALL(*backgroundHandlerMock, setColor(_)).Times(0);

    BackgroundBuilderTest builder(std::move(adaptor));
    builder.defaultSize().color(DEFAULT_COLOR).path(DEFAULT_PATH).adaptor(std::move(backgroundHandlerMock)).build();
}

TEST(BackgroundBuilderTest, Construct_DefaultColorPath_HandlerSetImageShouldBeCalled)
{
    auto adaptor = std::make_unique<NiceMock<MockFileSystemAdaptor>>();
    auto backgroundHandlerMock = std::make_unique<NiceMock<MockImageAdaptor>>();

    ON_CALL(*adaptor, isRegularFile(_)).WillByDefault(Return(true));
    EXPECT_CALL(*backgroundHandlerMock, setImage(_));

    BackgroundBuilderTest builder(std::move(adaptor));
    builder.defaultSize().color(DEFAULT_COLOR).path(DEFAULT_PATH).adaptor(std::move(backgroundHandlerMock)).build();
}

TEST(BackgroundBuilderTest, Construct_InvalidPath_ShouldThrowRunTimeError)
{
    auto adaptor = std::make_unique<NiceMock<MockFileSystemAdaptor>>();

    ON_CALL(*adaptor, isRegularFile(_)).WillByDefault(Return(false));

    BackgroundBuilderTest builder(std::move(adaptor));
    ASSERT_THROW(builder.defaultAdaptor().defaultSize().color({}).path({}).build(), std::runtime_error);
}

TEST(BackgroundBuilderTest, Construct_ValidPath_HandlerSetImageShouldNotBeCalled)
{
    auto adaptor = std::make_unique<NiceMock<MockFileSystemAdaptor>>();
    auto backgroundHandlerMock = std::make_unique<NiceMock<MockImageAdaptor>>();
    FilePath fullPath(DEFAULT_RESOURCES_DIR + "/" + DEFAULT_PATH);

    ON_CALL(*adaptor, isRegularFile(fullPath)).WillByDefault(Return(true));
    EXPECT_CALL(*backgroundHandlerMock, setImage(fullPath.string()));

    BackgroundBuilderTest builder(std::move(adaptor));
    builder.defaultSize().adaptor(std::move(backgroundHandlerMock)).color({}).path(DEFAULT_PATH).build();
}

