#include "MediaContainerBuilderTests.hpp"

using namespace testing;

TEST_P(MediaContainerBuilderSizeTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(MediaContainerBuilderTest().defaultVisibleMedia().width(GetParam().width).height(GetParam().height).build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MediaContainerBuilderSizeTest, ::testing::ValuesIn(invalidMediaContainerSizes));

TEST(MediaContainerBuilderTest, Construct_VisibleMedia_HandlerAddMediaWithPosShouldBeCalled)
{
    auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();

    EXPECT_CALL(*container, addMedia(_, _, _));

    MediaContainerBuilderTest().fakeContainer(std::move(container)).defaultVisibleMedia().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_VisibleMedia_HandlerAddMediaWithoutPosShouldNotBeCalled)
{
    auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();

    EXPECT_CALL(*container, addMedia(_)).Times(0);

    MediaContainerBuilderTest().fakeContainer(std::move(container)).defaultVisibleMedia().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_InvisibleMedia_HandlerAddMediaWithoutPosShouldBeCalled)
{
    auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();

    EXPECT_CALL(*container, addMedia(_));

    MediaContainerBuilderTest().fakeContainer(std::move(container)).defaultInvisibleMedia().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_InvisibleMedia_HandlerAddMediaWithPosShouldNotBeCalled)
{
    auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();

    EXPECT_CALL(*container, addMedia(_, _, _)).Times(0);

    MediaContainerBuilderTest().fakeContainer(std::move(container)).defaultInvisibleMedia().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_CustomZorder_MediaContainerWithCustomZorderCreated)
{
    auto container = MediaContainerBuilderTest().defaultVisibleMedia().defaultSize().zorder(DEFAULT_ZORDER + 1).build();

    ASSERT_EQ(container->zorder(), DEFAULT_ZORDER + 1);
}
