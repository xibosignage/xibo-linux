#include "MainLayoutBuilderTests.hpp"

using namespace testing;

TEST(MainLayoutBuilderSizeTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    auto adaptor = std::make_unique<NiceMock<MockOverlayAdaptor>>();

    EXPECT_CALL(*adaptor, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    MainLayoutBuilderTest().adaptor(std::move(adaptor)).defaultSize().build();
}

TEST_P(MainLayoutBuilderSizeTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(MainLayoutBuilderTest().width(GetParam().width).height(GetParam().height).build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutBuilderSizeTest, ::testing::ValuesIn(invalidMediaContainerSizes));

TEST(MainLayoutBuilderTest, Construct_WithContinaer_HandlerAddMediaContainerWithPos)
{
    auto adaptor = std::make_unique<NiceMock<MockOverlayAdaptor>>();

    EXPECT_CALL(*adaptor, addChild(_, _, _, _, _));

    MainLayoutBuilderTest().adaptor(std::move(adaptor)).defaultSize().build();
}

TEST(MainLayoutBuilderTest, Construct_WithBackground_HandlerSetBackground)
{
    auto adaptor = std::make_unique<NiceMock<MockOverlayAdaptor>>();

    EXPECT_CALL(*adaptor, addMainChild(_));

    MainLayoutBuilderTest().adaptor(std::move(adaptor)).defaultSize().build();
}
