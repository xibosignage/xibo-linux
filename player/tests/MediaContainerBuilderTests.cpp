#include "MediaContainerBuilderTests.hpp"

using namespace testing;

TEST(MediaContainerBuilderTest, Construct_ValidSize_HandlerSetSizeShouldBeCalled)
{
    auto containerHandlerMock = std::make_unique<NiceMock<MockFixedLayoutAdaptor>>();

    EXPECT_CALL(*containerHandlerMock, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    MediaContainerBuilderTest().defaultVisibleMedia().defaultTimer().defaultSize().adaptor(std::move(containerHandlerMock)).build();
}

TEST_P(MediaContainerBuilderSizeTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(MediaContainerBuilderTest().defaultVisibleMedia().defaultTimer().defaultAdaptor().width(GetParam().width).height(GetParam().height).build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MediaContainerBuilderSizeTest, ::testing::ValuesIn(invalidMediaContainerSizes));

TEST(MediaContainerBuilderTest, Construct_VisibleMedia_HandlerAddMediaWithPosShouldBeCalled)
{
    auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();

    EXPECT_CALL(*container, addMedia(_, _, _));

    MediaContainerBuilderTest().fakeContainer(std::move(container)).defaultVisibleMedia().defaultTimer().defaultAdaptor().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_VisibleMedia_HandlerAddMediaWithoutPosShouldNotBeCalled)
{
    auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();

    EXPECT_CALL(*container, addMedia(_)).Times(0);

    MediaContainerBuilderTest().fakeContainer(std::move(container)).defaultVisibleMedia().defaultTimer().defaultAdaptor().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_InvisibleMedia_HandlerAddMediaWithoutPosShouldBeCalled)
{
    auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();

    EXPECT_CALL(*container, addMedia(_));

    MediaContainerBuilderTest().fakeContainer(std::move(container)).defaultInvisibleMedia().defaultTimer().defaultAdaptor().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_InvisibleMedia_HandlerAddMediaWithPosShouldNotBeCalled)
{
    auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();

    EXPECT_CALL(*container, addMedia(_, _, _)).Times(0);

    MediaContainerBuilderTest().fakeContainer(std::move(container)).defaultInvisibleMedia().defaultTimer().defaultAdaptor().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_CustomZorder_MediaContainerWithCustomZorderCreated)
{
    auto container = MediaContainerBuilderTest().defaultVisibleMedia().defaultTimer().defaultAdaptor().defaultSize().zorder(DEFAULT_ZORDER + 1).build();

    ASSERT_EQ(container->zorder(), DEFAULT_ZORDER + 1);
}
