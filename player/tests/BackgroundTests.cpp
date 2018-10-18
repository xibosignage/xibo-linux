#include "BackgroundTests.hpp"

using namespace ::testing;

TEST(BackgroundTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [background, backgroundHandlerStub] = constructBackground();

    ASSERT_EQ(&background->handler(), backgroundHandlerStub);
}

TEST(BackgroundTest, Width_HandlerReturnsDefaultWidth_BackgroundWidthEqualsDefault)
{
    auto [background, backgroundHandlerStub] = constructBackground();

    ON_CALL(*backgroundHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(background->width(), DEFAULT_WIDTH);
}

TEST(BackgroundTest, Height_HandlerReturnsDefaultHeight_BackgroundHeightEqualsDefault)
{
    auto [background, backgroundHandlerStub] = constructBackground();

    ON_CALL(*backgroundHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(background->height(), DEFAULT_HEIGHT);
}

TEST(BackgroundTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [background, backgroundHandlerMock] = constructBackground();

    EXPECT_CALL(*backgroundHandlerMock, show());

    background->show();
}

TEST(BackgroundTest, Scale_Default_HandlerScaleShouldBeCalled)
{
    auto [background, backgroundHandlerMock] = constructBackground();

    EXPECT_CALL(*backgroundHandlerMock, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    background->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}
