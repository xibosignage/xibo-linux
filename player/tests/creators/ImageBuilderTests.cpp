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

TEST(ImageBuilderTest, Construct_InvalidPath_ShouldThrowRunTimeError)
{
    ImageBuilderTest builder;

    ON_CALL(builder.filesystem(), isRegularFile(_)).WillByDefault(Return(false));

    ASSERT_THROW(builder.defaultImageProps().path(DEFAULT_PATH.string()).build(), std::runtime_error);
}
