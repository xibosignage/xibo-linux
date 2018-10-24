#include "MediaContainerTests.hpp"

using namespace ::testing;

TEST(MediaContainerTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    auto containerHandlerMock = std::make_unique<NiceMock<MockFixedLayoutAdaptor>>();

    EXPECT_CALL(*containerHandlerMock, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructContainer(std::move(containerHandlerMock));
}

TEST(MediaContainerTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [container, containerHandlerStub] = constructContainer();

    ASSERT_EQ(&container->handler(), containerHandlerStub);
}

TEST(MediaContainerTest, Width_HandlerReturnsDefaultWidth_ContainerWidthEqualsDefault)
{
    auto [container, containerHandlerStub] = constructContainer();

    ON_CALL(*containerHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(container->width(), DEFAULT_WIDTH);
}

TEST(MediaContainerTest, Height_HandlerReturnsDefaultHeight_ContainerHeightEqualsDefault)
{
    auto [container, containerHandlerStub] = constructContainer();

    ON_CALL(*containerHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(container->height(), DEFAULT_HEIGHT);
}

TEST(MediaContainerTest, Zorder_Default_ContainerZorderEquals0)
{
    auto [layout, containerHandlerStub] = constructContainer();

    ASSERT_EQ(layout->zorder(), DEFAULT_ZORDER);
}

TEST(MediaContainerTest, AddMediaWithCoords_Valid_HandlerAddChildShouldBeCalled)
{
    auto [container, containerHandlerMock] = constructContainerWithoutMedia();
    auto stubMedia = createMediaWithPos();

    EXPECT_CALL(*containerHandlerMock, addChild(_, DEFAULT_XPOS, DEFAULT_YPOS));

    container->addMedia(unique(stubMedia), DEFAULT_XPOS, DEFAULT_YPOS);
}

TEST(MediaContainerTest, AddMediaWithCoords_Valid_MediaConnectShouldBeCalled)
{
    auto [container, containerHandlerStub] = constructContainerWithoutMedia();
    auto mockMedia = createMediaWithPos();

    EXPECT_CALL(*mockMedia, connect(_));

    container->addMedia(unique(mockMedia), DEFAULT_XPOS, DEFAULT_YPOS);
}

TEST(MediaContainerTest, AddMediaWithoutCoords_Valid_HandlerAddChildShouldNotBeCalled)
{
    auto [container, containerHandlerMock] = constructContainerWithoutMedia();

    EXPECT_CALL(*containerHandlerMock, addChild(_, _, _)).Times(0);

    container->addMedia(unique(createMedia()));
}

TEST(MediaContainerTest, AddMediaWithoutCoords_Valid_MediaConnectShouldBeCalled)
{
    auto [container, containerHandlerStub] = constructContainerWithoutMedia();
    auto mockMedia = createMedia();

    EXPECT_CALL(*mockMedia, connect(_));

    container->addMedia(unique(mockMedia));
}

TEST(MediaContainerTest, Scale_Default_HandlerScaleShouldBeCalled)
{
    auto [container, containerHandlerMock] = constructContainer();

    EXPECT_CALL(*containerHandlerMock, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    container->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST(MediaContainerTest, Scale_MediaWithPos_MediaScaleShouldBeCalled)
{
    auto [container, containerHandlerMock] = constructContainerWithoutMedia();
    auto mockMedia = createMediaWithPos();
    container->addMedia(unique(mockMedia), DEFAULT_XPOS, DEFAULT_YPOS);

    EXPECT_CALL(*mockMedia, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    container->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST(MediaContainerTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [container, containerHandlerMock] = constructContainer();

    EXPECT_CALL(*containerHandlerMock, show());

    container->show();
}

TEST(MediaContainerTest, Show_DefaultMediaDuration_TimerStartOnceShouldBeCalled)
{
    auto timerMock = new NiceMock<MockTimerProvider>;
    auto [container, containerHandlerStub] = constructContainerWithoutMedia(unique(timerMock));
    auto stubMedia = createMedia();

    ON_CALL(*stubMedia, duration()).WillByDefault(Return(DEFAULT_DURATION));
    EXPECT_CALL(*timerMock, startOnce(DEFAULT_DURATION * MSECS, _));

    container->addMedia(unique(stubMedia));
    container->show();
}

TEST(MediaContainerTest, Show_MediaDurationEquals0_TimerStartOnceShouldNotBeCalled)
{
    auto timerMock = new NiceMock<MockTimerProvider>;
    auto [container, containerHandlerStub] = constructContainerWithoutMedia(unique(timerMock));
    auto stubMedia = createMedia();

    ON_CALL(*stubMedia, duration()).WillByDefault(Return(0));
    EXPECT_CALL(*timerMock, startOnce(_, _)).Times(0);

    container->addMedia(unique(stubMedia));
    container->show();
}

TEST(MediaContainerTest, Show_With1Media_MediaStartShouldBeCalled)
{
    auto [container, containerHandlerStub] = constructContainerWithoutMedia();
    auto mockMedia = createMedia();
    container->addMedia(unique(mockMedia));

    EXPECT_CALL(*mockMedia, start());

    container->show();
}

TEST(MediaContainerTest, Show_With2Media_FirstMediaStartShouldBeCalled)
{
    auto [container, containerHandlerStub] = constructContainerWithoutMedia();
    auto mockMediaFirst = createMedia();
    container->addMedia(unique(mockMediaFirst));
    container->addMedia(unique(createMedia()));

    EXPECT_CALL(*mockMediaFirst, start());

    container->show();
}

TEST(MediaContainerTest, Show_With2Media_SecondMediaStartShouldNotBeCalled)
{
    auto [container, containerHandlerStub] = constructContainerWithoutMedia();
    container->addMedia(unique(createMedia()));
    auto mockMediaSecond = createMedia();
    container->addMedia(unique(mockMediaSecond));

    EXPECT_CALL(*mockMediaSecond, start()).Times(0);

    container->show();
}
