#include "ImageBuilderTests.hpp"

using namespace std::string_literals;

TEST(ImageBuilderTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    ImageBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    builder.defaultSize().defaultImageProps().defaultBaseProps().build();
}

TEST(ImageBuilderTest, Construct_Default_HandlerSetImageShouldBeCalled)
{
    ImageBuilderTest builder;
    FilePath fullPath(DEFAULT_RESOURCES_DIR / DEFAULT_PATH);

    EXPECT_CALL(builder.adaptor(), setImage(fullPath.string()));

    builder.defaultSize().defaultImageProps().defaultBaseProps().build();
}

TEST(ImageBuilderTest, Construct_InvalidScaleType_InvaldiArgShouldBeThrown)
{
    ImageBuilderTest builder;
    ASSERT_THROW(builder.defaultSize().defaultBaseProps().scaleType("invalid"s).align(DEFAULT_ALIGN).valign(DEFAULT_VALIGN).build(), std::invalid_argument);
}

TEST(ImageBuilderTest, Construct_InvalidAlign_InvaldiArgShouldBeThrown)
{
    ImageBuilderTest builder;
    ASSERT_THROW(builder.defaultSize().defaultBaseProps().scaleType(DEFAULT_SCALE_TYPE).align("invalid"s).valign(DEFAULT_VALIGN).build(), std::invalid_argument);
}

TEST(ImageBuilderTest, Construct_InvalidValign_InvaldiArgShouldBeThrown)
{
    ImageBuilderTest builder;
    ASSERT_THROW(builder.defaultSize().defaultBaseProps().scaleType(DEFAULT_SCALE_TYPE).align(DEFAULT_ALIGN).valign("invalid"s).build(), std::invalid_argument);
}

TEST(ImageBuilderTest, Construct_NoProps_DefaultPropsShouldBeSet)
{
    ImageBuilderTest builder;
    auto image = builder.defaultSize().defaultBaseProps().build();

//    ASSERT_EQ(image->scaleType(), ImageProperties::ScaleType::Center);
//    ASSERT_EQ(image->align(), ImageProperties::Align::Center);
//    ASSERT_EQ(image->valign(), ImageProperties::Valign::Middle);
}
