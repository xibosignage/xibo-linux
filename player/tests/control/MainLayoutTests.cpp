#include "MainLayoutTests.hpp"

using namespace ::testing;

TEST_F(MainLayoutTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto layout = constructLayout();

    ASSERT_EQ(&layout->handler(), &adaptor());
}

TEST_F(MainLayoutTest, Width_HandlerReturnsDefaultWidth_LayoutWidthEqualsDefault)
{
    auto layout = constructLayout();

    ON_CALL(adaptor(), width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(layout->width(), DEFAULT_WIDTH);
}

TEST_F(MainLayoutTest, Height_HandlerReturnsDefaultHeight_LayoutHeightEqualsDefault)
{
    auto layout = constructLayout();

    ON_CALL(adaptor(), height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(layout->height(), DEFAULT_HEIGHT);
}

TEST_F(MainLayoutTest, SetBackground_Valid_HandlerAddMainChildShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(adaptor(), addMainChild(_));

    layout->setBackground(createBackground());
}

TEST_F(MainLayoutTest, SetBackground_BackgroundWidthNotEqualLayoutWidth_InvalidArgErrorShouldBeCalled)
{
    auto layout = constructLayout();
    auto stubBackground = createBackground();

    ON_CALL(*stubBackground, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->setBackground(std::move(stubBackground)), std::invalid_argument);
}

TEST_F(MainLayoutTest, SetBackground_BackgroundHeightNotEqualLayoutHeight_InvalidArgErrorShouldBeCalled)
{
    auto layout = constructLayout();
    auto stubBackground = createBackground();

    ON_CALL(*stubBackground, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->setBackground(std::move(stubBackground)), std::invalid_argument);
}

TEST_F(MainLayoutTest, SetBackground_AlreadySetBefore_HandlerAddMainChild1TimeCalledRemoveMainChild1TimeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(adaptor(), addMainChild(_)).Times(1);
    EXPECT_CALL(adaptor(), removeMainChild()).Times(1);

    layout->setBackground(createBackground());
}

TEST_F(MainLayoutTest, AddMediaContainer_Add1_HandlerAddChildShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(adaptor(), addChild(_, _, _ ,_ , _));

    layout->addMediaContainer(createMediaContainer(), DEFAULT_XPOS, DEFAULT_YPOS);
}

TEST_F(MainLayoutTest, AddMediaContainer_ContainerWidthGreaterThanLayoutWidth_InvalidArgErrorShouldBeThrown)
{
    auto layout = constructLayout();
    auto stubContainer = createMediaContainer();

    ON_CALL(*stubContainer, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->addMediaContainer(std::move(stubContainer), DEFAULT_XPOS, DEFAULT_YPOS), std::invalid_argument);
}

TEST_F(MainLayoutTest, AddMediaContainer_ContainerHeightGreaterThanLayoutWidth_InvalidArgErrorShouldBeThrown)
{
    auto layout = constructLayout();
    auto stubContainer = createMediaContainer();

    ON_CALL(*stubContainer, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->addMediaContainer(std::move(stubContainer), DEFAULT_XPOS, DEFAULT_YPOS), std::invalid_argument);
}

TEST_F(MainLayoutTest, Scale_Default_HandlerScaleShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    layout->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(MainLayoutTest, Scale_WithContainer_ContainerScaleShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(container(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    layout->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(MainLayoutTest, Scale_WithBackground_BackgroundScaleShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(background(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    layout->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(MainLayoutTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(adaptor(), show());

    ON_CALL(adaptor(), isShown()).WillByDefault(Return(false));
    layout->show();
}

TEST_F(MainLayoutTest, Show_CallTwice_HandlerShowShouldBeCalledOnce)
{
    auto layout = constructLayout();

    EXPECT_CALL(adaptor(), show()).Times(1);

    layout->show();
    ON_CALL(adaptor(), isShown()).WillByDefault(Return(true));
    layout->show();
}

TEST_F(MainLayoutTest, Show_WithBackground_BackgroundShowShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(background(), show());

    ON_CALL(adaptor(), isShown()).WillByDefault(Return(false));
    layout->show();
}

TEST_F(MainLayoutTest, Show_WithBackgroundTwice_BackgroundShowShouldBeCalledOnce)
{
    auto layout = constructLayout();

    EXPECT_CALL(background(), show()).Times(1);

    layout->show();
    ON_CALL(adaptor(), isShown()).WillByDefault(Return(true));
    layout->show();
}

TEST_F(MainLayoutTest, Show_WithMediaContainer_MediaContainerShowShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(container(), show());

    ON_CALL(adaptor(), isShown()).WillByDefault(Return(false));
    layout->show();
}

TEST_F(MainLayoutTest, Show_WithMediaContainerTwice_MediaContainerShowShouldBeCalledOnce)
{
    auto layout = constructLayout();

    EXPECT_CALL(container(), show()).Times(1);

    layout->show();
    ON_CALL(adaptor(), isShown()).WillByDefault(Return(true));
    layout->show();
}

TEST_P(MainLayoutReorderTest, Show_With3MediaContainers_HandlerReorderChildShouldBeCalledWithCorrectOrder)
{
    auto layout = constructLayout();

    for(size_t index = 0; index != GetParam().size(); ++index)
        EXPECT_CALL(adaptor(), reorderChild(Ref(container(index)->handler()), static_cast<int>(index)));

    layout->show();
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutReorderTest, ::testing::ValuesIn(zorders));
