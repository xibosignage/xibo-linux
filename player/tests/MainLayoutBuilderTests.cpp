#include "MainLayoutBuilderTests.hpp"

using namespace testing;

TEST(MainLayoutBuilderTest, Construct_ValidSize_HandlerSetSizeShouldBeCalled)
{
    auto layoutHandlerMock = std::make_unique<NiceMock<MockOverlayAdaptor>>();

    EXPECT_CALL(*layoutHandlerMock, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    MainLayoutBuilderTest().defaultBackground().defaultContainers().defaultSize().adaptor(std::move(layoutHandlerMock)).build();
}

TEST_P(MainLayoutBuilderSizeTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(MainLayoutBuilderTest().defaultBackground().defaultContainers().defaultAdaptor().width(GetParam().width).height(GetParam().height).build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutBuilderSizeTest, ::testing::ValuesIn(invalidMediaContainerSizes));

TEST(MainLayoutBuilderTest, Construct_WithContinaer_HandlerAddMediaContainerWithPos)
{
    auto layout = fake_construct<MockMainLayout, MockOverlayAdaptor>();

    EXPECT_CALL(*layout, addMediaContainer(_, _, _));

    MainLayoutBuilderTest().fakeLayout(std::move(layout)).defaultContainers().defaultBackground().defaultAdaptor().defaultSize().build();
}

TEST(MainLayoutBuilderTest, Construct_WithBackground_HandlerSetBackground)
{
    auto layout = fake_construct<MockMainLayout, MockOverlayAdaptor>();

    EXPECT_CALL(*layout, setBackground(_));

    MainLayoutBuilderTest().fakeLayout(std::move(layout)).defaultContainers().defaultBackground().defaultAdaptor().defaultSize().build();
}
