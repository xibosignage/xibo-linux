#include "MediaContainerBuilderTests.hpp"

using namespace testing;

TEST(MediaContainerBuilderSizeTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    auto adaptor = std::make_unique<testing::NiceMock<MockFixedLayoutAdaptor>>();

    EXPECT_CALL(*adaptor, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    MediaContainerBuilderTest().adaptor(std::move(adaptor)).defaultVisibleMedia().defaultSize().build();
}

TEST_P(MediaContainerBuilderSizeTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(MediaContainerBuilderTest().defaultVisibleMedia().width(GetParam().width).height(GetParam().height).build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MediaContainerBuilderSizeTest, ::testing::ValuesIn(invalidMediaContainerSizes));

TEST(MediaContainerBuilderTest, Construct_VisibleMedia_HandlerAddMediaWithPosShouldBeCalled)
{
    auto adaptor = std::make_unique<testing::NiceMock<MockFixedLayoutAdaptor>>();

    EXPECT_CALL(*adaptor, addChild(_, _, _));

    MediaContainerBuilderTest().adaptor(std::move(adaptor)).defaultVisibleMedia().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_InvisibleMedia_HandlerAddMediaWithPosShouldNotBeCalled)
{
    auto adaptor = std::make_unique<testing::NiceMock<MockFixedLayoutAdaptor>>();

    EXPECT_CALL(*adaptor, addChild(_, _, _)).Times(0);

    MediaContainerBuilderTest().adaptor(std::move(adaptor)).defaultInvisibleMedia().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_CustomZorder_MediaContainerWithCustomZorderCreated)
{
    auto container = MediaContainerBuilderTest().defaultVisibleMedia().defaultSize().zorder(DEFAULT_ZORDER + 1).build();

    ASSERT_EQ(container->zorder(), DEFAULT_ZORDER + 1);
}
