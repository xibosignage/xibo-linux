#include "test_utils.hpp"

#include "creators/MediaContainerBuilder.hpp"

#include "control/MediaContainer.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockFixedLayoutAdaptor.hpp"
#include "mocks/MockMedia.hpp"
#include "mocks/MockWidgetAdaptor.hpp"

using namespace ::testing;

NiceMock<MockMedia>* createMedia()
{
    auto handler = new NiceMock<MockWidgetAdaptor>;
    auto container = new NiceMock<MockMedia>(unique(handler));

    ON_CALL(*container, handler()).WillByDefault(ReturnRef(*handler));

    return container;
}

auto constructContainer()
{
    auto [container, handler] = construct<MediaContainer, MockFixedLayoutAdaptor>(DEFAULT_ZORDER, DEFAULT_LOOP);
    container->addMedia(unique(createMedia()));
    return std::pair{container, handler};
}

auto constructContainerWithoutMedia()
{
    return construct<MediaContainer, MockFixedLayoutAdaptor>(DEFAULT_ZORDER, DEFAULT_LOOP);
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
    auto stubMedia = createMedia();

    EXPECT_CALL(*containerHandlerMock, addChild(_, DEFAULT_X_POS, DEFAULT_Y_POS));

    container->addMedia(unique(stubMedia), DEFAULT_X_POS, DEFAULT_Y_POS);
}

TEST(MediaContainerTest, AddMediaWithCoords_Valid_MediaConnectShouldBeCalled)
{
    auto [container, containerHandlerStub] = constructContainerWithoutMedia();
    auto mockMedia = createMedia();

    EXPECT_CALL(*mockMedia, connect(_));

    container->addMedia(unique(mockMedia), DEFAULT_X_POS, DEFAULT_Y_POS);
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

TEST(MediaContainerTest, RemoveAllMedia_Default_HandlerRemoveChildrenShouldBeCalled)
{
    auto [container, containerHandlerMock] = constructContainer();

    EXPECT_CALL(*containerHandlerMock, removeChildren());

    container->removeAllMedia();
}

TEST(MediaContainerTest, Scale_Default_HandlerScaleShouldBeCalled)
{
    auto [container, containerHandlerMock] = constructContainer();

    EXPECT_CALL(*containerHandlerMock, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    container->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST(MediaContainerTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [container, containerHandlerMock] = constructContainer();

    EXPECT_CALL(*containerHandlerMock, show());

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
