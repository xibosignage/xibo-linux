#include "ImageTests.hpp"

using namespace ::testing;

TEST(ImageTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [image, imageHandlerStub] = constructImage();

    ASSERT_EQ(&image->handler(), imageHandlerStub);
}

TEST(ImageTest, Width_HandlerReturnsDefaultWidth_ImageWidthEqualsDefault)
{
    auto [image, imageHandlerStub] = constructImage();

    ON_CALL(*imageHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(image->width(), DEFAULT_WIDTH);
}

TEST(ImageTest, Height_HandlerReturnsDefaultHeight_ImageHeightEqualsDefault)
{
    auto [image, imageHandlerStub] = constructImage();

    ON_CALL(*imageHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(image->height(), DEFAULT_HEIGHT);
}

TEST(ImageTest, Scale_Default_ImageScaleShouldBeCalled)
{
    auto [image, imageHandlerMock] = constructImage();

    EXPECT_CALL(*imageHandlerMock, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    image->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST(ImageTest, ScaleType_Default_EqualsDefault)
{
    auto [image, imageHandlerStub] = constructImage();

    ASSERT_EQ(image->scaleType(), DEFAULT_SCALE_TYPE);
}

TEST(ImageTest, Align_Default_EqualsDefault)
{
    auto [image, imageHandlerStub] = constructImage();

    ASSERT_EQ(image->align(), DEFAULT_ALIGN);
}

TEST(ImageTest, Valign_Default_EqualsDefault)
{
    auto [image, imageHandlerStub] = constructImage();

    ASSERT_EQ(image->valign(), DEFAULT_VALIGN);
}

TEST(ImageTest, Start_Default_HandlerShowShouldBeCalled)
{
    auto [image, imageHandlerMock] = constructImage();

    EXPECT_CALL(*imageHandlerMock, show());

    image->start();
}

TEST(ImageTest, Stop_Default_HandlerHideShouldBeCalled)
{
    auto [image, imageHandlerMock] = constructImage();

    EXPECT_CALL(*imageHandlerMock, hide());

    image->stop();
}
