#include "MediaContainerTests.hpp"

using namespace ::testing;

TEST_F(MediaContainerTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto container = constructContainer();

    ASSERT_EQ(&container->handler(), &adaptor());
}

TEST_F(MediaContainerTest, Width_HandlerReturnsDefaultWidth_ContainerWidthEqualsDefault)
{
    auto container = constructContainer();

    ON_CALL(adaptor(), width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(container->width(), DEFAULT_WIDTH);
}

TEST_F(MediaContainerTest, Height_HandlerReturnsDefaultHeight_ContainerHeightEqualsDefault)
{
    auto container = constructContainer();

    ON_CALL(adaptor(), height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(container->height(), DEFAULT_HEIGHT);
}

TEST_F(MediaContainerTest, Zorder_Default_ContainerZorderEquals0)
{
    auto container = constructContainer();

    ASSERT_EQ(container->zorder(), DEFAULT_ZORDER);
}

TEST_F(MediaContainerTest, AddMediaWithCoords_Valid_HandlerAddChildShouldBeCalled)
{
    auto container = constructContainer();

    EXPECT_CALL(adaptor(), addChild(_, DEFAULT_XPOS, DEFAULT_YPOS));

    container->addMedia(createMediaWithPos(), DEFAULT_XPOS, DEFAULT_YPOS);
}

TEST_F(MediaContainerTest, AddMediaWithCoords_Valid_MediaConnectShouldBeCalled)
{
    auto container = constructContainer();
    auto mockMedia = createMediaWithPos();

    EXPECT_CALL(*mockMedia, connect(_));

    container->addMedia(std::move(mockMedia), DEFAULT_XPOS, DEFAULT_YPOS);
}

TEST_F(MediaContainerTest, AddMediaWithoutCoords_Valid_HandlerAddChildShouldNotBeCalled)
{
    auto container = constructContainer();

    EXPECT_CALL(adaptor(), addChild(_, _, _)).Times(0);

    container->addMedia(createMedia());
}

TEST_F(MediaContainerTest, AddMediaWithoutCoords_Valid_MediaConnectShouldBeCalled)
{
    auto container = constructContainer();
    auto mockMedia = createMedia();

    EXPECT_CALL(*mockMedia, connect(_));

    container->addMedia(std::move(mockMedia));
}

TEST_F(MediaContainerTest, Scale_Default_HandlerScaleShouldBeCalled)
{
    auto container = constructContainer();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    container->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(MediaContainerTest, Scale_MediaWithPos_MediaScaleShouldBeCalled)
{
    auto container = constructContainer();

    EXPECT_CALL(secondVisibleMedia(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    container->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(MediaContainerTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto container = constructContainer();

    EXPECT_CALL(adaptor(), show());

    container->show();
}

TEST_F(MediaContainerTest, Show_DefaultMediaDuration_TimerStartOnceShouldBeCalled)
{
    auto container = constructContainer();

    ON_CALL(firstInvisibleMedia(), duration()).WillByDefault(Return(DEFAULT_DURATION));
    EXPECT_CALL(timer(), startOnce(DEFAULT_DURATION * MSECS, _));

    container->show();
}

TEST_F(MediaContainerTest, Show_MediaDurationEquals0_TimerStartOnceShouldNotBeCalled)
{
    auto container = constructContainer();

    ON_CALL(firstInvisibleMedia(), duration()).WillByDefault(Return(0));
    EXPECT_CALL(timer(), startOnce(_, _)).Times(0);

    container->show();
}

TEST_F(MediaContainerTest, Show_With2Media_FirstMediaStartShouldBeCalled)
{
    auto container = constructContainer();

    EXPECT_CALL(firstInvisibleMedia(), start());

    container->show();
}

TEST_F(MediaContainerTest, Show_With2Media_SecondMediaStartShouldNotBeCalled)
{
    auto container = constructContainer();

    EXPECT_CALL(secondVisibleMedia(), start()).Times(0);

    container->show();
}
