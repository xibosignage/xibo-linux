#include "ImageBuilderTests.hpp"
#include "media/Image.hpp"

using namespace std::string_literals;
using namespace testing;

TEST(ImageBuilderTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    ImageBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    builder.defaultImageProps().build();
}

TEST(ImageBuilderTest, Construct_Scaled_HandlerLoadImageWithPreserveAspectRatioShouldBeCalled)
{
    ImageBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), loadImage(DEFAULT_FULL_PATH, true));

    builder.scaleType("center"s).align(DEFAULT_ALIGN).valign(DEFAULT_VALIGN).build();
}

TEST(ImageBuilderTest, Construct_Scaled_HandlerLoadImageWithoutPreserveAspectRatioShouldBeCalled)
{
    ImageBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), loadImage(DEFAULT_FULL_PATH, false));

    builder.scaleType("stretch"s).align(DEFAULT_ALIGN).valign(DEFAULT_VALIGN).build();
}

TEST(ImageBuilderTest, Construct_InvalidScaleType_InvaldiArgShouldBeThrown)
{
    ImageBuilderTest builder;
    ASSERT_THROW(builder.scaleType("invalid"s).align(DEFAULT_ALIGN).valign(DEFAULT_VALIGN).build(), std::invalid_argument);
}

TEST(ImageBuilderTest, Construct_InvalidAlign_InvaldiArgShouldBeThrown)
{
    ImageBuilderTest builder;
    ASSERT_THROW(builder.scaleType(DEFAULT_SCALE_TYPE).align("invalid"s).valign(DEFAULT_VALIGN).build(), std::invalid_argument);
}

TEST(ImageBuilderTest, Construct_InvalidValign_InvaldiArgShouldBeThrown)
{
    ImageBuilderTest builder;
    ASSERT_THROW(builder.scaleType(DEFAULT_SCALE_TYPE).align(DEFAULT_ALIGN).valign("invalid"s).build(), std::invalid_argument);
}

TEST(ImageBuilderTest, Construct_ScaleTypeCenter_ImageScaleTypeEqualsScaled)
{
    auto image = ImageBuilderTest().scaleType("center"s).align(DEFAULT_ALIGN).valign(DEFAULT_VALIGN).build();

    ASSERT_EQ(image->scaleType(), MediaGeometry::ScaleType::Scaled);
}

TEST(ImageBuilderTest, Construct_ScaleTypeStretch_ImageScaleTypeEqualsStretch)
{
    auto image = ImageBuilderTest().scaleType("stretch"s).align(DEFAULT_ALIGN).valign(DEFAULT_VALIGN).build();

    ASSERT_EQ(image->scaleType(), MediaGeometry::ScaleType::Stretch);
}

TEST(ImageBuilderTest, Construct_AlignCenter_ImageAlignEqualsCenter)
{
    auto image = ImageBuilderTest().scaleType(DEFAULT_SCALE_TYPE).align("center"s).valign(DEFAULT_VALIGN).build();

    ASSERT_EQ(image->align(), MediaGeometry::Align::Center);
}

TEST(ImageBuilderTest, Construct_AlignRight_ImageAlignEqualsRight)
{
    auto image = ImageBuilderTest().scaleType(DEFAULT_SCALE_TYPE).align("right"s).valign(DEFAULT_VALIGN).build();

    ASSERT_EQ(image->align(), MediaGeometry::Align::Right);
}

TEST(ImageBuilderTest, Construct_AlignLeft_ImageAlignEqualsLeft)
{
    auto image = ImageBuilderTest().scaleType(DEFAULT_SCALE_TYPE).align("left"s).valign(DEFAULT_VALIGN).build();

    ASSERT_EQ(image->align(), MediaGeometry::Align::Left);
}

TEST(ImageBuilderTest, Construct_ValignMiddle_ImageValignEqualsMiddle)
{
    auto image = ImageBuilderTest().scaleType(DEFAULT_SCALE_TYPE).align(DEFAULT_ALIGN).valign("middle"s).build();

    ASSERT_EQ(image->valign(), MediaGeometry::Valign::Middle);
}

TEST(ImageBuilderTest, Construct_ValignTop_ImageValignEqualsTop)
{
    auto image = ImageBuilderTest().scaleType(DEFAULT_SCALE_TYPE).align(DEFAULT_ALIGN).valign("top"s).build();

    ASSERT_EQ(image->valign(), MediaGeometry::Valign::Top);
}

TEST(ImageBuilderTest, Construct_ValignBottom_ImageValignEqualsBottom)
{
    auto image = ImageBuilderTest().scaleType(DEFAULT_SCALE_TYPE).align(DEFAULT_ALIGN).valign("bottom"s).build();

    ASSERT_EQ(image->valign(), MediaGeometry::Valign::Bottom);
}

TEST(ImageBuilderTest, Construct_InvalidPath_ShouldThrowRunTimeError)
{
    ImageBuilderTest builder;

    ON_CALL(builder.filesystem(), isRegularFile(_)).WillByDefault(Return(false));

    ASSERT_THROW(builder.defaultImageProps().path(DEFAULT_PATH.string()).build(), std::runtime_error);
}
