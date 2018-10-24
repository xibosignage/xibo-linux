#include "BackgroundTests.hpp"

using namespace ::testing;

using MediaTypes = ::testing::Types<ImageBackground, OneColorBackground>;
TYPED_TEST_CASE(BackgroundTest, MediaTypes);

TEST(OneColorBackgroundTest, Construct_ValidColor_HandlerSetColorShouldBeCalled)
{
    auto backgroundHandlerMock = std::make_unique<NiceMock<MockImageAdaptor>>();

    EXPECT_CALL(*backgroundHandlerMock, setColor(DEFAULT_COLOR));

    constructBackground<OneColorBackground>(std::move(backgroundHandlerMock));
}

TEST(ImageBackgroundTest, Construct_ValidPath_HandlerSetImageShouldBeCalled)
{
    auto backgroundHandlerMock = std::make_unique<NiceMock<MockImageAdaptor>>();

    EXPECT_CALL(*backgroundHandlerMock, setImage(DEFAULT_PATH.string()));

    constructBackground<ImageBackground>(std::move(backgroundHandlerMock));
}

TYPED_TEST(BackgroundTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    auto backgroundHandlerMock = std::make_unique<NiceMock<MockImageAdaptor>>();

    EXPECT_CALL(*backgroundHandlerMock, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructBackground<TypeParam>(std::move(backgroundHandlerMock));
}

TYPED_TEST(BackgroundTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [background, backgroundHandlerStub] = constructBackground<TypeParam>();

    ASSERT_EQ(&background->handler(), backgroundHandlerStub);
}

TYPED_TEST(BackgroundTest, Width_HandlerReturnsDefaultWidth_BackgroundWidthEqualsDefault)
{
    auto [background, backgroundHandlerStub] = constructBackground<TypeParam>();

    ON_CALL(*backgroundHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(background->width(), DEFAULT_WIDTH);
}

TYPED_TEST(BackgroundTest, Height_HandlerReturnsDefaultHeight_BackgroundHeightEqualsDefault)
{
    auto [background, backgroundHandlerStub] = constructBackground<TypeParam>();

    ON_CALL(*backgroundHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(background->height(), DEFAULT_HEIGHT);
}

TYPED_TEST(BackgroundTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [background, backgroundHandlerMock] = constructBackground<TypeParam>();

    EXPECT_CALL(*backgroundHandlerMock, show());

    background->show();
}

TYPED_TEST(BackgroundTest, Scale_Default_HandlerScaleShouldBeCalled)
{
    auto [background, backgroundHandlerMock] = constructBackground<TypeParam>();

    EXPECT_CALL(*backgroundHandlerMock, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    background->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}
