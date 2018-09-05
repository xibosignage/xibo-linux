#include "test_utils.hpp"

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

class BackgroundTest : public TestWithParam<Size> { };
class BackgroundValidColorTest : public TestWithParam<Color> { };
class BackgroundInvalidColorTest :  public TestWithParam<std::string> { };

const std::vector<Color> validColors =
    {{"#000", 255}, {"#FFF", 4294967295}, {"#123", 287454207}, {"#1FA", 301968127}, {"#fff", 4294967295}, {"#fFf", 4294967295},
    {"#000000", 255}, {"#12Ff32", 318714623}, {"#4312AA", 1125296895}, {"#aaFFbb", 2868886527}, {"#FfFfFf", 4294967295},
    {"#000000FF", 255}, {"#00fd00FF", 16580863}, {"#FFFFFFFF", 4294967295}, {"#FFF000ff", 4293918975}};

const std::vector<std::string> invalidColors =
    {"#sss", "121", "asdfasdf", "11G", "#12H", "#1111",
    "#11", "#1", "1", "#dfdsaw", "#11111", "#1111111"
    "#111111111", "zzzzzz", "test me", "invalid",
    "#dfdfd1s", "#1111d", "", " ", "   "};


TEST_P(BackgroundTest, Constructor_InvalidSize_ShouldThrowRunTimeError)
{
    ASSERT_THROW(construct_background(GetParam().width, GetParam().height), std::runtime_error);
}

TEST_P(BackgroundTest, SetSize_InvalidSize_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_background();

    ASSERT_THROW(background->setSize(GetParam().width, GetParam().height), std::runtime_error);
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundTest, ::testing::ValuesIn(invalidSizes));

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

TEST(BackgroundTest, SetColor_InvalidColor_HandlerSetColorShouldNotBeCalled)
{
    auto [background, mockImage] = construct_background();

    EXPECT_CALL(*mockImage, setColor(_)).Times(0);

    EXPECT_ANY_THROW(background->setColor("#sss"));
}

TEST_P(BackgroundValidColorTest, SetColor_Valid_HandlerSetColorShouldBeCalled)
{
    auto [background, mockImage] = construct_background();

    EXPECT_CALL(*mockImage, setColor(GetParam().numberColor));

    background->setColor(GetParam().strColor);
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundValidColorTest, ::testing::ValuesIn(validColors));

TEST_P(BackgroundInvalidColorTest, SetColor_Invalid_InvalidArgumentShouldBeThrown)
{
    auto [background, stubImage] = construct_background();

    ASSERT_THROW(background->setColor(GetParam()), std::invalid_argument);
}

TEST_P(BackgroundInvalidColorTest, SetColor_Invalid_HandlerSetColorShouldNotBeCalled)
{
    auto [background, mockImage] = construct_background();

    EXPECT_CALL(*mockImage, setColor(_)).Times(0);

    EXPECT_ANY_THROW(background->setColor(GetParam()););
}

INSTANTIATE_TEST_CASE_P(Suite, BackgroundInvalidColorTest, ::testing::ValuesIn(invalidColors));

