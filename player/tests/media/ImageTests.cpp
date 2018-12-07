#include "ImageTests.hpp"

using namespace ::testing;

TEST_F(ImageTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto image = constructImage();

    ASSERT_EQ(&image->handler(), &adaptor());
}

TEST_F(ImageTest, Width_HandlerReturnsDefaultWidth_ImageWidthEqualsDefault)
{
    auto image = constructImage();

    ON_CALL(adaptor(), width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(image->width(), DEFAULT_WIDTH);
}

TEST_F(ImageTest, Height_HandlerReturnsDefaultHeight_ImageHeightEqualsDefault)
{
    auto image = constructImage();

    ON_CALL(adaptor(), height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(image->height(), DEFAULT_HEIGHT);
}

TEST_F(ImageTest, Scale_Default_ImageScaleShouldBeCalled)
{
    auto image = constructImage();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    image->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(ImageTest, ScaleType_Default_EqualsDefault)
{
    auto image = constructImage();

    ASSERT_EQ(image->scaleType(), DEFAULT_SCALE_TYPE);
}

TEST_F(ImageTest, Align_Default_EqualsDefault)
{
    auto image = constructImage();

    ASSERT_EQ(image->align(), DEFAULT_ALIGN);
}

TEST_F(ImageTest, Valign_Default_EqualsDefault)
{
    auto image = constructImage();

    ASSERT_EQ(image->valign(), DEFAULT_VALIGN);
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
