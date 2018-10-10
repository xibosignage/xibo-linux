#include "MediaContainerBuilderTests.hpp"

using namespace testing;

TEST(MediaContainerBuilderTest, Construct_ValidSize_HandlerSetSizeShouldBeCalled)
{
    auto containerHandlerMock = std::make_unique<NiceMock<MockFixedLayoutAdaptor>>();

    EXPECT_CALL(*containerHandlerMock, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    MediaContainerBuilderTest().defaultVisibleMedia().defaultSize().adaptor(std::move(containerHandlerMock)).build();
}

TEST_P(MediaContainerBuilderSizeTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(MediaContainerBuilderTest().defaultVisibleMedia().defaultAdaptor().width(GetParam().width).height(GetParam().height).build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MediaContainerBuilderSizeTest, ::testing::ValuesIn(invalidMediaContainerSizes));

TEST(MediaContainerBuilderTest, Construct_VisibleMedia_HandlerAddMediaWithPos)
{
    auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();

    EXPECT_CALL(*container, addMedia(_, _, _)).Times(1);

    MediaContainerBuilderTest().fakeContainer(std::move(container)).defaultVisibleMedia().defaultAdaptor().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_InvisibleMedia_HandlerAddMediaWithoutPos)
{
    auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();

    EXPECT_CALL(*container, addMedia(_)).Times(1);

    MediaContainerBuilderTest().fakeContainer(std::move(container)).defaultInvisibleMedia().defaultAdaptor().defaultSize().build();
}

TEST(MediaContainerBuilderTest, Construct_CustomZorder_MediaContainerWithCustomZorderCreated)
{
    auto container = MediaContainerBuilderTest().defaultVisibleMedia().defaultAdaptor().defaultSize().zorder(DEFAULT_ZORDER + 1).build();

    ASSERT_EQ(container->zorder(), DEFAULT_ZORDER + 1);
}
