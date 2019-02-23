#include "BackgroundTests.hpp"

using namespace ::testing;

using MediaTypes = ::testing::Types<ImageBackground, OneColorBackground>;
TYPED_TEST_CASE(BackgroundTest, MediaTypes);

const auto invalidBackgroundSizes = invalidSizes<MIN_DISPLAY_WIDTH, MIN_DISPLAY_HEIGHT>;

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


TEST_F(ImageBackgroundTest, Construct_WithColorAndPath_HandlerLoadImageWithPreserveAspectRatioShouldBeCalled)
{
    EXPECT_CALL(adaptor(), loadImage(DEFAULT_FULL_PATH, true));

    constructBackground();
}

TEST_F(ImageBackgroundTest, Construct_WithColorAndPath_ImageBackgroungShouldBeCreated)
{
    auto background = constructBackground();

    ASSERT_TRUE(dynamic_cast<ImageBackground*>(background.get()));
}

TEST_F(ImageBackgroundTest, Construct_InvalidPath_ShouldThrowRunTimeError)
{
    ON_CALL(filesystem(), isRegularFile(_)).WillByDefault(Return(false));

    ASSERT_THROW(constructBackground(), std::runtime_error);
}

TEST_P(ImageBackgroundSizeTest, ImageBackgroundConstruct_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(constructBackground(GetParam().width, GetParam().height, DEFAULT_PATH.string(), DEFAULT_COLOR), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, ImageBackgroundSizeTest, ::testing::ValuesIn(invalidBackgroundSizes));

TEST_P(OneColorBackgroundSizeTest, OneColorBackgroundConstruct_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(constructBackground(GetParam().width, GetParam().height, DEFAULT_PATH.string(), DEFAULT_COLOR), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, OneColorBackgroundSizeTest, ::testing::ValuesIn(invalidBackgroundSizes));

TEST_P(OneColorBackgroundValidColorTest, OneColorBackgroundConstruct_ValidColor_HandlerSetColorShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setColor(GetParam().numberColor));

    constructBackground(DEFAULT_WIDTH, DEFAULT_HEIGHT, {}, GetParam().strColor);
}

INSTANTIATE_TEST_CASE_P(Suite, OneColorBackgroundValidColorTest, ::testing::ValuesIn(validColors));

TEST_P(OneColorBackgroundInvalidColorTest, OneColorBackgroundConstruct_InvalidColor_InvalidArgumentShouldBeThrown)
{
    ASSERT_THROW(constructBackground(DEFAULT_WIDTH, DEFAULT_HEIGHT, {}, GetParam()), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, OneColorBackgroundInvalidColorTest, ::testing::ValuesIn(invalidColors));

TEST_F(OneColorBackgroundTest, OneColorBackgroundConstruct_NoColor_ColorWillBeDefaulted)
{
    EXPECT_CALL(adaptor(), setColor(255));

    constructBackground(DEFAULT_WIDTH, DEFAULT_HEIGHT, {}, {});
}

TYPED_TEST(BackgroundTest, ConstructWithColor_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(this->adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    this->constructBackground();
}

TYPED_TEST(BackgroundTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto background = this->constructBackground();

    ASSERT_EQ(&background->handler(), &this->adaptor());
}

TYPED_TEST(BackgroundTest, WidthHeight_Default_BackgroundWidthHeightEqualsDefault)
{
    auto background = this->constructBackground();

    ASSERT_EQ(background->width(), DEFAULT_WIDTH);
    ASSERT_EQ(background->height(), DEFAULT_HEIGHT);
}

TYPED_TEST(BackgroundTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto background = this->constructBackground();

    EXPECT_CALL(this->adaptor(), show());

    background->show();
}

TYPED_TEST(BackgroundTest, Scale_Default_HandlerScaleShouldBeCalled)
{
    auto background = this->constructBackground();

    EXPECT_CALL(this->adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    background->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}
