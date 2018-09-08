#include "test_utils.hpp"

#include "control/MediaContainer.hpp"
#include "media/MediaVisitor.hpp"

#include "MockFixedLayoutAdaptor.hpp"
#include "MockMedia.hpp"
#include "MockWidgetAdaptor.hpp"

using namespace ::testing;

const int DEFAULT_ZORDER = 0;
const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

auto construct_container(int zorder)
{
    return construct<MediaContainer, MockFixedLayoutAdaptor>(DEFAULT_WIDTH, DEFAULT_HEIGHT, zorder);
}

auto construct_container(int width, int height)
{
    return construct<MediaContainer, MockFixedLayoutAdaptor>(width, height, DEFAULT_ZORDER);
}

auto construct_container()
{
    return construct<MediaContainer, MockFixedLayoutAdaptor>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_ZORDER);
}

NiceMock<MockMedia>* createMedia()
{
    auto background = new NiceMock<MockMedia>;
    auto stubWidget = std::make_unique<NiceMock<MockWidgetAdaptor>>();

    ON_CALL(*background, handler()).WillByDefault(ReturnRef(*stubWidget));

    return background;
}

const auto invalidMediaContainerSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_WIDTH, MAX_DISPLAY_HEIGHT, MIN_HEIGHT>;

class MediaContainerTest : public TestWithParam<Size> { };

TEST_P(MediaContainerTest, Constructor_InvalidSize_ShouldThrowRunTimeError)
{
    ASSERT_THROW(construct_container(GetParam().width, GetParam().height), std::runtime_error);
}

TEST_P(MediaContainerTest, SetSize_InvalidSize_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_container();

    ASSERT_THROW(background->setSize(GetParam().width, GetParam().height), std::runtime_error);
}

INSTANTIATE_TEST_CASE_P(Suite, MediaContainerTest, ::testing::ValuesIn(invalidMediaContainerSizes));

TEST(MediaContainerTest, Constructor_Default_HandlerSetSizeShouldBeCalled)
{
    auto mockFixed = std::make_unique<NiceMock<MockFixedLayoutAdaptor>>();

    EXPECT_CALL(*mockFixed, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    std::make_shared<MediaContainer>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_ZORDER, std::move(mockFixed));
}

TEST(MediaContainerTest, Width_Default_Equals640)
{
    auto [layout, stubFixed] = construct_container();

    ON_CALL(*stubFixed, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(layout->width(), DEFAULT_WIDTH);
}

TEST(MediaContainerTest, Height_Default_Equals480)
{
    auto [layout, stubFixed] = construct_container();

    ON_CALL(*stubFixed, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(layout->height(), DEFAULT_HEIGHT);
}

TEST(MediaContainerTest, Zorder_Default_Equals0)
{
    auto [layout, stubFixed] = construct_container();

    ASSERT_EQ(layout->zorder(), DEFAULT_ZORDER);
}

TEST(MediaContainerTest, AddMediaWithCoords_Null_HandlerAddChildShouldNotBeCalled)
{
    auto [container, mockFixed] = construct_container();

    EXPECT_CALL(*mockFixed, addChild(_, _, _)).Times(0);

    container->addMedia(nullptr, DEFAULT_X_POS, DEFAULT_Y_POS);
}

TEST(MediaContainerTest, AddMediaWithCoords_Valid_HandlerAddChildShouldBeCalled)
{
    auto [container, mockFixed] = construct_container();
    auto stubMedia = createMedia();

    EXPECT_CALL(*mockFixed, addChild(_, DEFAULT_X_POS, DEFAULT_Y_POS));

    container->addMedia(unique(stubMedia), DEFAULT_X_POS, DEFAULT_Y_POS);
}

TEST(MediaContainerTest, AddMediaWithCoords_Valid_MediaConnectShouldBeCalled)
{
    auto [container, stubFixed] = construct_container();
    auto mockMedia = createMedia();

    EXPECT_CALL(*mockMedia, connect(_));

    container->addMedia(unique(mockMedia), DEFAULT_X_POS, DEFAULT_Y_POS);
}

TEST(MediaContainerTest, AddMediaWithoutCoords_Null_HandlerAddChildShouldNotBeCalled)
{
    auto [container, mockFixed] = construct_container();

    EXPECT_CALL(*mockFixed, addChild(_, _, _)).Times(0);

    container->addMedia(nullptr);
}

TEST(MediaContainerTest, AddMediaWithoutCoords_Valid_HandlerAddChildShouldNotBeCalled)
{
    auto [container, mockFixed] = construct_container();

    EXPECT_CALL(*mockFixed, addChild(_, _, _)).Times(0);

    container->addMedia(unique(createMedia()));
}

TEST(MediaContainerTest, AddMediaWithoutCoords_Valid_MediaConnectShouldBeCalled)
{
    auto [container, stubFixed] = construct_container();
    auto mockMedia = createMedia();

    EXPECT_CALL(*mockMedia, connect(_));

    container->addMedia(unique(mockMedia));
}

TEST(MediaContainerTest, RemoveAllMedia_Default_HandlerRemoveChildrenShouldBeCalled)
{
    auto [container, mockFixed] = construct_container();

    EXPECT_CALL(*mockFixed, removeChildren());

    container->removeAllMedia();
}

TEST(MediaContainerTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [container, mockFixed] = construct_container();

    EXPECT_CALL(*mockFixed, show());

    container->show();
}

TEST(MediaContainerTest, Show_With1Media_MediaStartShouldBeCalled)
{
    auto [container, stubFixed] = construct_container();
    auto mockMedia = createMedia();
    container->addMedia(unique(mockMedia));

    EXPECT_CALL(*mockMedia, start());

    container->show();
}

TEST(MediaContainerTest, Show_With2Media_FirstMediaStartShouldBeCalled)
{
    auto [container, stubFixed] = construct_container();
    auto mockMediaFirst = createMedia();
    container->addMedia(unique(mockMediaFirst));
    container->addMedia(unique(createMedia()));

    EXPECT_CALL(*mockMediaFirst, start());

    container->show();
}

TEST(MediaContainerTest, Show_With2Media_SecondMediaStartShouldNotBeCalled)
{
    auto [container, stubFixed] = construct_container();
    container->addMedia(unique(createMedia()));
    auto mockMediaSecond = createMedia();
    container->addMedia(unique(mockMediaSecond));

    EXPECT_CALL(*mockMediaSecond, start()).Times(0);

    container->show();
}
