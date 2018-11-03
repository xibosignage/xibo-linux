#include "BackgroundTests.hpp"

using namespace ::testing;

using MediaTypes = ::testing::Types<ImageBackground, OneColorBackground>;
TYPED_TEST_CASE(BackgroundTest, MediaTypes);

TYPED_TEST(BackgroundTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto background = this->constructBackground();

    ASSERT_EQ(&background->handler(), &this->adaptor());
}

TYPED_TEST(BackgroundTest, Width_HandlerReturnsDefaultWidth_BackgroundWidthEqualsDefault)
{
    auto background = this->constructBackground();

    ON_CALL(this->adaptor(), width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(background->width(), DEFAULT_WIDTH);
}

TYPED_TEST(BackgroundTest, Height_HandlerReturnsDefaultHeight_BackgroundHeightEqualsDefault)
{
    auto background = this->constructBackground();

    ON_CALL(this->adaptor(), height()).WillByDefault(Return(DEFAULT_HEIGHT));

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
