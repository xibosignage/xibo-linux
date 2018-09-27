#include "test_utils.hpp"

#include "control/Background.hpp"

#include "mocks/MockImageAdaptor.hpp"

using namespace ::testing;

auto construct_background()
{
    auto [background, adaptor] = construct<Background, MockImageAdaptor>();
    background->setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    return std::pair{background, adaptor};
}

auto construct_background_without_size()
{
    auto [background, adaptor] = construct<Background, MockImageAdaptor>();
    return std::pair{background, adaptor};
}

struct Color
{
    std::string strColor;
    uint numberColor;
};

const auto invalidBackgroundSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, MIN_DISPLAY_HEIGHT>;

class BackgroundTest : public TestWithParam<Size> { };
class BackgroundValidColorTest : public TestWithParam<Color> { };
class BackgroundInvalidColorTest :  public TestWithParam<std::string> { };

const std::vector<Color> validColors =
    {{"#000", 255}, {"#FFF", 4294967295}, {"#123", 287454207}, {"#1FA", 301968127}, {"#fff", 4294967295}, {"#fFf", 4294967295},
     {"#000000", 255}, {"#12Ff32", 318714623}, {"#4312AA", 1125296895}, {"#aaFFbb", 2868886527}, {"#FfFfFf", 4294967295},
     {"#000000FF", 255}, {"#00fd00FF", 16580863}, {"#FFFFFFFF", 4294967295}, {"#FFF000ff", 4293918975}, {"#FFFF", 4294967295},
     {"#123F", 287454207}, {"#1234", 287454020}};

const std::vector<std::string> invalidColors =
    {"#sss", "121", "asdfasdf", "11G", "#12H", "#111V",
    "#11", "#1", "1", "#dfdsaw", "#11111", "#1111111"
    "#111111111", "zzzzzz", "test me", "invalid",
    "#dfdfd1s", "#1111d", "", " ", "   "};


TEST_P(BackgroundTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    auto [background, backgroundHandlerStub] = construct_background_without_size();

    ASSERT_THROW(background->setSize(GetParam().width, GetParam().height), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundTest, ::testing::ValuesIn(invalidBackgroundSizes));

TEST(BackgroundTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [background, backgroundHandlerStub] = construct_background();

    ASSERT_EQ(&background->handler(), backgroundHandlerStub);
}

TEST(BackgroundTest, Width_HandlerReturnsDefaultWidth_BackgroundWidthEqualsDefault)
{
    auto [background, backgroundHandlerStub] = construct_background();

    ON_CALL(*backgroundHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(background->width(), DEFAULT_WIDTH);
}

TEST(BackgroundTest, Height_HandlerReturnsDefaultHeight_BackgroundHeightEqualsDefault)
{
    auto [background, backgroundHandlerStub] = construct_background();

    ON_CALL(*backgroundHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(background->height(), DEFAULT_HEIGHT);
}

TEST(BackgroundTest, SetSize_Width200Height200_HandlerSetSize200Width200HeightCalled)
{
    auto [background, backgroundHandlerMock] = construct_background();

    EXPECT_CALL(*backgroundHandlerMock, setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    background->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(BackgroundTest, SetSize_Width200Height200_BackgroundWidthEquals200)
{
    auto [background, backgroundHandlerStub] = construct_background();

    ON_CALL(*backgroundHandlerStub, width()).WillByDefault(Return(NEW_DEFAULT_WIDTH));

    background->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(background->width(), NEW_DEFAULT_WIDTH);
}

TEST(BackgroundTest, SetSize_Width200Height200_BackgroundHeightEquals200)
{
    auto [background, backgroundHandlerStub] = construct_background();

    ON_CALL(*backgroundHandlerStub, height()).WillByDefault(Return(NEW_DEFAULT_HEIGHT));

    background->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(background->height(), NEW_DEFAULT_HEIGHT);
}

TEST(BackgroundTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [background, backgroundHandlerMock] = construct_background();

    EXPECT_CALL(*backgroundHandlerMock, show());

    background->show();
}

TEST(BackgroundTest, SetImage_Default_HandlerSetImageShouldBeCalled)
{
    auto [background, backgroundHandlerMock] = construct_background();
    const int BUFFER_SIZE = 5;
    uint8_t fakeData[BUFFER_SIZE];

    EXPECT_CALL(*backgroundHandlerMock, setImage(fakeData, BUFFER_SIZE));

    background->setImage(fakeData, BUFFER_SIZE);
}

TEST_P(BackgroundValidColorTest, SetColor_ValidColor_HandlerSetColorShouldBeCalled)
{
    auto [background, backgroundHandlerMock] = construct_background();

    EXPECT_CALL(*backgroundHandlerMock, setColor(GetParam().numberColor));

    background->setColor(GetParam().strColor);
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundValidColorTest, ::testing::ValuesIn(validColors));

TEST_P(BackgroundInvalidColorTest, SetColor_InvalidColor_InvalidArgumentShouldBeThrown)
{
    auto [background, backgroundHandlerStub] = construct_background();

    ASSERT_THROW(background->setColor(GetParam()), std::invalid_argument);
}

TEST_P(BackgroundInvalidColorTest, SetColor_InvalidColor_HandlerSetColorShouldNotBeCalled)
{
    auto [background, backgroundHandlerMock] = construct_background();

    EXPECT_CALL(*backgroundHandlerMock, setColor(_)).Times(0);

    EXPECT_ANY_THROW(background->setColor(GetParam()));
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundInvalidColorTest, ::testing::ValuesIn(invalidColors));
