#include "ImageTests.hpp"

using namespace ::testing;
using namespace std::string_literals;

TEST_F(ImageTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructImage();
}

TEST_F(ImageTest, Construct_InvalidScaleType_InvaldiArgShouldBeThrown)
{
    ASSERT_THROW(constructImage("invalid"s, DEFAULT_ALIGN, DEFAULT_VALIGN), std::invalid_argument);
}

TEST_F(ImageTest, Construct_InvalidAlign_InvaldiArgShouldBeThrown)
{
    ASSERT_THROW(constructImage(DEFAULT_SCALE_TYPE, "invalid"s, DEFAULT_VALIGN), std::invalid_argument);
}

TEST_F(ImageTest, Construct_InvalidValign_InvaldiArgShouldBeThrown)
{
    ASSERT_THROW(constructImage(DEFAULT_SCALE_TYPE, DEFAULT_ALIGN, "invalid"s), std::invalid_argument);
}

TEST_F(ImageTest, Construct_Scaled_HandlerLoadImageWithPreserveAspectRatioShouldBeCalled)
{
    EXPECT_CALL(adaptor(), loadImage(DEFAULT_FULL_PATH, true));

    constructImage("center"s, DEFAULT_ALIGN, DEFAULT_VALIGN);
}

TEST_F(ImageTest, Construct_Scaled_HandlerLoadImageWithoutPreserveAspectRatioShouldBeCalled)
{
    EXPECT_CALL(adaptor(), loadImage(DEFAULT_FULL_PATH, false));

    constructImage("stretch"s, DEFAULT_ALIGN, DEFAULT_VALIGN);
}

TEST_F(ImageTest, Construct_NoAlignValignScaleType_AlignValignScaleTypeEqualsDefault)
{
    auto image = constructImage({}, {}, {});

    ASSERT_EQ(image->scaleType(), MediaGeometry::ScaleType::Scaled);
    ASSERT_EQ(image->align(), MediaGeometry::Align::Center);
    ASSERT_EQ(image->valign(), MediaGeometry::Valign::Middle);
}

TEST_F(ImageTest, Construct_ScaleTypeCenter_ImageScaleTypeEqualsScaled)
{
    auto image = constructImage("center"s, DEFAULT_ALIGN, DEFAULT_VALIGN);

    ASSERT_EQ(image->scaleType(), MediaGeometry::ScaleType::Scaled);
}

TEST_F(ImageTest, Construct_ScaleTypeStretch_ImageScaleTypeEqualsStretch)
{
    auto image = constructImage("stretch"s, DEFAULT_ALIGN, DEFAULT_VALIGN);

    ASSERT_EQ(image->scaleType(), MediaGeometry::ScaleType::Stretch);
}

TEST_F(ImageTest, Construct_AlignCenter_ImageAlignEqualsCenter)
{
    auto image = constructImage(DEFAULT_SCALE_TYPE, "center"s, DEFAULT_VALIGN);

    ASSERT_EQ(image->align(), MediaGeometry::Align::Center);
}

TEST_F(ImageTest, Construct_AlignRight_ImageAlignEqualsRight)
{
    auto image = constructImage(DEFAULT_SCALE_TYPE, "right"s, DEFAULT_VALIGN);

    ASSERT_EQ(image->align(), MediaGeometry::Align::Right);
}

TEST_F(ImageTest, Construct_AlignLeft_ImageAlignEqualsLeft)
{
    auto image = constructImage(DEFAULT_SCALE_TYPE, "left"s, DEFAULT_VALIGN);

    ASSERT_EQ(image->align(), MediaGeometry::Align::Left);
}

TEST_F(ImageTest, Construct_ValignMiddle_ImageValignEqualsMiddle)
{
    auto image = constructImage(DEFAULT_SCALE_TYPE, DEFAULT_ALIGN, "middle"s);

    ASSERT_EQ(image->valign(), MediaGeometry::Valign::Middle);
}

TEST_F(ImageTest, Construct_ValignTop_ImageValignEqualsTop)
{
    auto image = constructImage(DEFAULT_SCALE_TYPE, DEFAULT_ALIGN, "top"s);

    ASSERT_EQ(image->valign(), MediaGeometry::Valign::Top);
}

TEST_F(ImageTest, Construct_ValignBottom_ImageValignEqualsBottom)
{
    auto image = constructImage(DEFAULT_SCALE_TYPE, DEFAULT_ALIGN, "bottom"s);

    ASSERT_EQ(image->valign(), MediaGeometry::Valign::Bottom);
}

TEST_F(ImageTest, Handler_Default_EqualsToPassedAdaptor)
{
    auto image = constructImage();

    ASSERT_EQ(&image->handler(), &adaptor());
}

TEST_F(ImageTest, WidthHeight_Default_ImageWidthHeightEqualsDefault)
{
    auto image = constructImage();

    ON_CALL(this->adaptor(), width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
    ON_CALL(this->adaptor(), height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));

    ASSERT_EQ(image->width(), DEFAULT_WIDTH);
    ASSERT_EQ(image->height(), DEFAULT_HEIGHT);
}

TEST_F(ImageTest, Scale_Default_HandlerScaleShouldBeCalled)
{
    auto image = constructImage();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    image->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(ImageTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto image = constructImage();

    EXPECT_CALL(adaptor(), show());

    image->show();
}

TEST_F(ImageTest, Hide_Default_HandlerHideShouldBeCalled)
{
    auto image = constructImage();

    EXPECT_CALL(adaptor(), hide());

    image->hide();
}

TEST_F(ImageTest, HandlerEvent_StartMediaEvent_HandlerShowShouldBeCalled)
{
    auto image = constructImage();

    EXPECT_CALL(adaptor(), show());

    image->handleEvent(StartMediaEvent{});
}

TEST_F(ImageTest, HandlerEvent_StopMediaEvent_HandlerHideShouldBeCalled)
{
    auto image = constructImage();

    EXPECT_CALL(adaptor(), hide());

    image->handleEvent(StopMediaEvent{});
}

TEST_F(ImageTest, HandlerEvent_ScaledMediaEvent_HandlerScaleShouldBeCalled)
{
    auto image = constructImage();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    image->handleEvent(ScaleMediaEvent{DEFAULT_XSCALE, DEFAULT_YSCALE});
}

