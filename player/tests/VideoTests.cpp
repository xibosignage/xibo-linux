#include "VideoTests.hpp"
#include "adaptors/IWidgetAdaptor.hpp"

using namespace testing;

TEST(VideoTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    auto videoHandlerMock = std::make_unique<NiceMock<MockVideoHandler>>();

    EXPECT_CALL(*videoHandlerMock, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructVideo(std::move(videoHandlerMock));
}

TEST(VideoTest, Construct_Default_HandlerLoadShouldBeCalled)
{
    auto videoHandlerMock = std::make_unique<NiceMock<MockVideoHandler>>();

    EXPECT_CALL(*videoHandlerMock, load(DEFAULT_PATH));

    constructVideo(std::move(videoHandlerMock));
}

TEST(VideoTest, Start_Default_HandlerShowShouldBeCalled)
{
    auto [video, videoHandlerMock] = constructVideo();

    EXPECT_CALL(*videoHandlerMock, play());

    video->start();
}

TEST(VideoTest, Stop_Default_HandlerHideShouldBeCalled)
{
    auto [video, videoHandlerMock] = constructVideo();

    EXPECT_CALL(*videoHandlerMock, stop());

    video->stop();
}

TEST(VideoTest, SetMuted_True_HandlerSetVolume0ShouldBeCalled)
{
    auto [video, videoHandlerMock] = constructVideo();

    EXPECT_CALL(*videoHandlerMock, setVolume(MIN_VOLUME));

    video->setMuted(true);
}

TEST(VideoTest, SetMuted_False_HandlerSetVolume100ShouldBeCalled)
{
    auto [video, videoHandlerMock] = constructVideo();

    EXPECT_CALL(*videoHandlerMock, setVolume(MAX_VOLUME));

    video->setMuted(false);
}

TEST(VideoTest, Width_HandlerReturnsDefaultWidth_VideoWidthEqualsDefault)
{
    auto [video, videoHandlerStub] = constructVideo();

    ON_CALL(*videoHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(video->width(), DEFAULT_WIDTH);
}

TEST(VideoTest, Height_HandlerReturnsDefaultHeight_VideoHeightEqualsDefault)
{
    auto [video, videoHandlerStub] = constructVideo();

    ON_CALL(*videoHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(video->height(), DEFAULT_HEIGHT);
}

TEST(VideoTest, Scale_Default_VideoScaleShouldBeCalled)
{
    auto [video, videoHandlerMock] = constructVideo();

    EXPECT_CALL(*videoHandlerMock, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    video->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}
