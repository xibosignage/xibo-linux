#include "test_utils.hpp"

#include "control/MediaContainer.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockFixedLayoutAdaptor.hpp"
#include "mocks/MockMedia.hpp"
#include "mocks/MockWidgetAdaptor.hpp"

using namespace ::testing;

const int DEFAULT_ZORDER = 0;
const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

auto construct_container(int zorder)
{
    return construct<MediaContainer, MockFixedLayoutAdaptor>(zorder);
}

auto construct_container()
{
    auto [container, adaptor] = construct<MediaContainer, MockFixedLayoutAdaptor>(DEFAULT_ZORDER);
    container->setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    return std::pair{container, adaptor};
}

auto construct_container_without_size()
{
    auto [container, adaptor] = construct<MediaContainer, MockFixedLayoutAdaptor>(DEFAULT_ZORDER);
    return std::pair{container, adaptor};
}

NiceMock<MockMedia>* createMedia()
{
    auto container = new NiceMock<MockMedia>;
    auto stubWidget = std::make_unique<NiceMock<MockWidgetAdaptor>>();

    ON_CALL(*container, handler()).WillByDefault(ReturnRef(*stubWidget));

    return container;
}

const auto invalidMediaContainerSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_WIDTH, MAX_DISPLAY_HEIGHT, MIN_HEIGHT>;

class MediaContainerTest : public TestWithParam<Size> { };

TEST_P(MediaContainerTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    auto [container, containerHandlerStub] = construct_container_without_size();

    ASSERT_THROW(container->setSize(GetParam().width, GetParam().height), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MediaContainerTest, ::testing::ValuesIn(invalidMediaContainerSizes));

TEST(MediaContainerTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [container, containerHandlerStub] = construct_container();

    ASSERT_EQ(&container->handler(), containerHandlerStub);
}

TEST(MediaContainerTest, Width_HandlerReturnsDefaultWidth_ContainerWidthEqualsDefault)
{
    auto [container, containerHandlerStub] = construct_container();

    ON_CALL(*containerHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(container->width(), DEFAULT_WIDTH);
}

TEST(MediaContainerTest, Height_HandlerReturnsDefaultHeight_ContainerHeightEqualsDefault)
{
    auto [container, containerHandlerStub] = construct_container();

    ON_CALL(*containerHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(container->height(), DEFAULT_HEIGHT);
}

TEST(MediaContainerTest, SetSize_Width200Height200_HandlerSetSizeWidth200Height200Called)
{
    auto [container, containerHandlerMock] = construct_container();

    EXPECT_CALL(*containerHandlerMock, setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    container->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(MediaContainerTest, SetSize_Width200Height200_ContainerWidthEquals200)
{
    auto [container, containerHandlerStub] = construct_container();

    ON_CALL(*containerHandlerStub, width()).WillByDefault(Return(NEW_DEFAULT_WIDTH));

    container->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(container->width(), NEW_DEFAULT_WIDTH);
}

TEST(MediaContainerTest, SetSize_Width200Height200_ContainerHeightEquals200)
{
    auto [container, containerHandlerStub] = construct_container();

    ON_CALL(*containerHandlerStub, height()).WillByDefault(Return(NEW_DEFAULT_HEIGHT));

    container->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(container->height(), NEW_DEFAULT_HEIGHT);
}

TEST(MediaContainerTest, Zorder_Default_ContainerZorderEquals0)
{
    auto [layout, containerHandlerStub] = construct_container();

    ASSERT_EQ(layout->zorder(), DEFAULT_ZORDER);
}

TEST(MediaContainerTest, AddMediaWithCoords_Valid_HandlerAddChildShouldBeCalled)
{
    auto [container, containerHandlerMock] = construct_container();
    auto stubMedia = createMedia();

    EXPECT_CALL(*containerHandlerMock, addChild(_, DEFAULT_X_POS, DEFAULT_Y_POS));

    container->addMedia(unique(stubMedia), DEFAULT_X_POS, DEFAULT_Y_POS);
}

TEST(MediaContainerTest, AddMediaWithCoords_Valid_MediaConnectShouldBeCalled)
{
    auto [container, containerHandlerStub] = construct_container();
    auto mockMedia = createMedia();

    EXPECT_CALL(*mockMedia, connect(_));

    container->addMedia(unique(mockMedia), DEFAULT_X_POS, DEFAULT_Y_POS);
}

TEST(MediaContainerTest, AddMediaWithoutCoords_Valid_HandlerAddChildShouldNotBeCalled)
{
    auto [container, containerHandlerMock] = construct_container();

    EXPECT_CALL(*containerHandlerMock, addChild(_, _, _)).Times(0);

    container->addMedia(unique(createMedia()));
}

TEST(MediaContainerTest, AddMediaWithoutCoords_Valid_MediaConnectShouldBeCalled)
{
    auto [container, containerHandlerStub] = construct_container();
    auto mockMedia = createMedia();

    EXPECT_CALL(*mockMedia, connect(_));

    container->addMedia(unique(mockMedia));
}

TEST(MediaContainerTest, RemoveAllMedia_Default_HandlerRemoveChildrenShouldBeCalled)
{
    auto [container, containerHandlerMock] = construct_container();

    EXPECT_CALL(*containerHandlerMock, removeChildren());

    container->removeAllMedia();
}

TEST(MediaContainerTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [container, containerHandlerMock] = construct_container();

    EXPECT_CALL(*containerHandlerMock, show());

    container->show();
}

TEST(MediaContainerTest, Show_With1Media_MediaStartShouldBeCalled)
{
    auto [container, containerHandlerStub] = construct_container();
    auto mockMedia = createMedia();
    container->addMedia(unique(mockMedia));

    EXPECT_CALL(*mockMedia, start());

    container->show();
}

TEST(MediaContainerTest, Show_With2Media_FirstMediaStartShouldBeCalled)
{
    auto [container, containerHandlerStub] = construct_container();
    auto mockMediaFirst = createMedia();
    container->addMedia(unique(mockMediaFirst));
    container->addMedia(unique(createMedia()));

    EXPECT_CALL(*mockMediaFirst, start());

    container->show();
}

TEST(MediaContainerTest, Show_With2Media_SecondMediaStartShouldNotBeCalled)
{
    auto [container, containerHandlerStub] = construct_container();
    container->addMedia(unique(createMedia()));
    auto mockMediaSecond = createMedia();
    container->addMedia(unique(mockMediaSecond));

    EXPECT_CALL(*mockMediaSecond, start()).Times(0);

    container->show();
}
