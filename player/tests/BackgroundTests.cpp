#include "test_utils.hpp"

#include "constants.hpp"
#include "control/Background.hpp"
#include "MockImageAdaptor.hpp"

using namespace ::testing;

auto construct_background(int width, int height)
{
    return construct<Background, MockImageAdaptor>(width, height);
}

auto construct_background()
{
    return construct<Background, MockImageAdaptor>(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

struct Color
{
    std::string strColor;
    uint numberColor;
};

class BackgroundTest : public Test { };
class BackgroundValidColor : public BackgroundTest, public WithParamInterface<Color> { };
class BackgroundInvalidColor :  public BackgroundTest, public WithParamInterface<std::string> { };


const std::vector<Color> validColors =
    {{"#000", 255}, {"#FFF", 4294967295}, {"#123", 287454207}, {"#1FA", 301968127}, {"#fff", 4294967295}, {"#fFf", 4294967295},
    {"#000000", 255}, {"#12Ff32", 318714623}, {"#4312AA", 1125296895}, {"#aaFFbb", 2868886527}, {"#FfFfFf", 4294967295},
    {"#000000FF", 255}, {"#00fd00FF", 16580863}, {"#FFFFFFFF", 4294967295}, {"#FFF000ff", 4293918975}};

const std::vector<std::string> invalidColors =
    {"#sss", "121", "asdfasdf", "11G", "#12H", "#1111",
    "#11", "#1", "1", "#dfdsaw", "#11111", "#1111111"
    "#111111111", "zzzzzz", "test me", "invalid",
    "#dfdfd1s", "#1111d", "", " ", "   "};


TEST(BackgroundTest, Constructor_NegativeWidth_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct_background(-1, DEFAULT_HEIGHT), std::runtime_error);
}

TEST(BackgroundTest, Constructor_NegativeHeigth_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct_background(DEFAULT_WIDTH, -1), std::runtime_error);
}

TEST(BackgroundTest, Constructor_WidthGreaterThanMaxWidth_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct_background(MAX_DISPLAY_WIDTH + 1, DEFAULT_HEIGHT), std::runtime_error);
}

TEST(BackgroundTest, Constructor_HeightGreaterThanMaxHeight_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct_background(DEFAULT_WIDTH, MAX_DISPLAY_HEIGHT + 1), std::runtime_error);
}

TEST(BackgroundTest, Constructor_WidthLesshanMinWidth_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct_background(MIN_DISPLAY_WIDTH - 1, DEFAULT_HEIGHT), std::runtime_error);
}

TEST(BackgroundTest, Constructor_HeightLessThanMinHeight_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct_background(DEFAULT_WIDTH, MIN_DISPLAY_HEIGHT - 1), std::runtime_error);
}

TEST(BackgroundTest, Constructor_VeryBigWidth_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct_background(std::numeric_limits<int>::max(), DEFAULT_HEIGHT), std::runtime_error);
}

TEST(BackgroundTest, Constructor_VeryBigHeight_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct_background(DEFAULT_WIDTH, std::numeric_limits<int>::max()), std::runtime_error);
}

TEST(BackgroundTest, Constructor_Default_HandlerSetSizeShouldBeCalled)
{
    auto mockImage = std::make_unique<NiceMock<MockImageAdaptor>>();

    EXPECT_CALL(*mockImage, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    std::make_shared<Background>(DEFAULT_WIDTH, DEFAULT_HEIGHT, std::move(mockImage));
}

TEST(BackgroundTest, Width_Default_EqualsDefaultWidth)
{
    auto [background, stubImage] = construct_background();

    ON_CALL(*stubImage, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(background->width(), DEFAULT_WIDTH);
}

TEST(BackgroundTest, Height_Default_EqualsDefaultHeight)
{
    auto [background, stubImage] = construct_background();

    ON_CALL(*stubImage, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(background->height(), DEFAULT_HEIGHT);
}

TEST(BackgroundTest, SetSize_Width200Height200_HandlerSetSizeShouldBeCalled)
{
    auto [background, mockImage] = construct_background();

    EXPECT_CALL(*mockImage, setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    background->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(BackgroundTest, SetSize_Width200Height200_WidthEquals200)
{
    auto [background, stubImage] = construct_background();

    ON_CALL(*stubImage, width()).WillByDefault(Return(NEW_DEFAULT_WIDTH));

    background->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(background->width(), NEW_DEFAULT_WIDTH);
}

TEST(BackgroundTest, SetSize_Width200Height200_HeightEquals200)
{
    auto [background, stubImage] = construct_background();

    ON_CALL(*stubImage, height()).WillByDefault(Return(NEW_DEFAULT_HEIGHT));

    background->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(background->height(), NEW_DEFAULT_HEIGHT);
}

TEST(BackgroundTest, SetSize_NegativeWidth_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->setSize(-1, NEW_DEFAULT_HEIGHT), std::runtime_error);
}

TEST(BackgroundTest, SetSize_NegativeHeigth_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->setSize(NEW_DEFAULT_WIDTH, -1), std::runtime_error);
}

TEST(BackgroundTest, SetSize_WidthGreaterThan_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->setSize(MAX_DISPLAY_WIDTH + 1, NEW_DEFAULT_HEIGHT), std::runtime_error);
}

TEST(BackgroundTest, SetSize_HeightGreaterThan_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->setSize(NEW_DEFAULT_WIDTH, MAX_DISPLAY_HEIGHT + 1), std::runtime_error);
}

TEST(BackgroundTest, SetSize_WidthLesshanMinWidth_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->setSize(MIN_DISPLAY_WIDTH - 1, DEFAULT_HEIGHT), std::runtime_error);
}

TEST(BackgroundTest, SetSize_HeightLessThanMinHeight_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->setSize(DEFAULT_WIDTH, MIN_DISPLAY_HEIGHT - 1), std::runtime_error);
}

TEST(BackgroundTest, SetSize_VeryBigWidth_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->setSize(std::numeric_limits<int>::max(), NEW_DEFAULT_HEIGHT), std::runtime_error);
}

TEST(BackgroundTest, SetSize_VeryBigHeight_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->setSize(NEW_DEFAULT_WIDTH, std::numeric_limits<int>::max()), std::runtime_error);
}

TEST(BackgroundTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [background, mockImage] = construct_background();

    EXPECT_CALL(*mockImage, show());

    background->show();
}

TEST(BackgroundTest, SetImage_Default_HandlerSetImageShouldBeCalled)
{
    auto [background, mockImage] = construct_background();
    const int BUFFER_SIZE = 5;
    uint8_t fakeData[BUFFER_SIZE];

    EXPECT_CALL(*mockImage, setImage(_, _));

    background->setImage(fakeData, BUFFER_SIZE);
}

TEST(BackgroundTest, HexColor_NotSet_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->hexColor(), std::runtime_error);
}

TEST(BackgroundTest, HexColor_Set_Equals000000FF)
{
    auto [background, stubImage] = construct_background();

    background->setColor("#000000FF");

    ASSERT_EQ(background->hexColor(), "#000000FF");
}

TEST(BackgroundTest, HexColorNumber_NotSet_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->hexColorNumber(), std::runtime_error);
}

TEST(BackgroundTest, HexColorNumber_Set_Equals255)
{
    auto [background, stubImage] = construct_background();

    background->setColor("#000000FF");

    ASSERT_EQ(background->hexColorNumber(), 255);
}

TEST(BackgroundTest, SetColor_InvalidColor_HandlerSetColorShouldNotBeCalled)
{
    auto [background, mockImage] = construct_background();

    EXPECT_CALL(*mockImage, setColor(_)).Times(0);

    EXPECT_ANY_THROW(background->setColor("#sss"));
}

TEST_P(BackgroundValidColor, SetColor_Valid_HandlerSetColorShouldBeCalled)
{
    auto [background, mockImage] = construct_background();

    EXPECT_CALL(*mockImage, setColor(GetParam().numberColor));

    background->setColor(GetParam().strColor);
}

TEST_P(BackgroundValidColor, HexColor_Valid_HexColorEqualsParam)
{
    auto [background, stubImage] = construct_background();

    background->setColor(GetParam().strColor);

    ASSERT_EQ(background->hexColor(), GetParam().strColor);
}

TEST_P(BackgroundValidColor, HexColorNumber_Valid_HexColorEqualsParamConvertedToNumber)
{
    auto [background, stubImage] = construct_background();

    background->setColor(GetParam().strColor);

    ASSERT_EQ(background->hexColorNumber(), GetParam().numberColor);
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundValidColor, ::testing::ValuesIn(validColors));

TEST_P(BackgroundInvalidColor, SetColor_Invalid_InvalidArgumentShouldBeThrown)
{
    auto [background, stubImage] = construct_background();

    EXPECT_THROW(background->setColor(GetParam()), std::invalid_argument);
}

TEST_P(BackgroundInvalidColor, SetColor_Invalid_HandlerSetColorShouldNotBeCalled)
{
    auto [background, mockImage] = construct_background();

    EXPECT_CALL(*mockImage, setColor(_)).Times(0);

    EXPECT_ANY_THROW(background->setColor(GetParam()););
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundInvalidColor, ::testing::ValuesIn(invalidColors));

