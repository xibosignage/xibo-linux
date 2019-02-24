#include "VideoTests.hpp"
#include "adaptors/IWidgetAdaptor.hpp"

using namespace testing;

TEST_F(VideoTest, Construct_Default_HandlerLoadShouldBeCalled)
{
    EXPECT_CALL(adaptor(), load(DEFAULT_FULL_PATH));

    constructVideo();
}

TEST_F(VideoTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructVideo();
}

TEST_F(VideoTest, Construct_NoMute_HandlerSetVolume100ShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setVolume(MAX_VOLUME));

    constructVideo({}, DEFAULT_VIDEO_LOOPED);
}

TEST_F(VideoTest, Construct_NoLoop_LoopEqualsDefault)
{
    auto video = constructVideo(DEFAULT_VIDEO_MUTED, {});

    ASSERT_EQ(video->looped(), DEFAULT_VIDEO_LOOPED);
}

TEST_F(VideoTest, Construct_MutedTrue_HandlerSetVolume0ShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setVolume(MIN_VOLUME));

    constructVideo(VideoOptions::Mute::Enable, DEFAULT_VIDEO_LOOPED);
}

TEST_F(VideoTest, Construct_MutedTrue_HandlerSetVolume100ShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setVolume(MAX_VOLUME));

    constructVideo(VideoOptions::Mute::Disable, DEFAULT_VIDEO_LOOPED);
}

TEST_F(VideoTest, Construct_NoLoop_VideoLoopedEqualsDefault)
{
    auto video = constructVideo(DEFAULT_VIDEO_MUTED, {});

    ASSERT_EQ(video->looped(), DEFAULT_VIDEO_LOOPED);
}

TEST_F(VideoTest, Construct_LoopTrue_VideoLoopedEqualsTrue)
{
    auto video = constructVideo(DEFAULT_VIDEO_MUTED, VideoOptions::Loop::Enable);

    ASSERT_EQ(video->looped(), VideoOptions::Loop::Enable);
}

TEST_F(VideoTest, Construct_LoopFalse_VideoLoopedEqualsFalse)
{
    auto video = constructVideo(DEFAULT_VIDEO_MUTED, VideoOptions::Loop::Disable);

    ASSERT_EQ(video->looped(), VideoOptions::Loop::Disable);
}

TEST_F(VideoTest, Construct_Default_ScaleTypeScaledAlignLeftValignTop)
{
    auto video = constructVideo();

    ASSERT_EQ(video->scaleType(), MediaGeometry::ScaleType::Scaled);
    ASSERT_EQ(video->align(), MediaGeometry::Align::Left);
    ASSERT_EQ(video->valign(), MediaGeometry::Valign::Top);
}

TEST_F(VideoTest, Play_Default_HandlerPlayShouldBeCalled)
{
    auto video = constructVideo();

    EXPECT_CALL(adaptor(), play());

    video->play();
}

TEST_F(VideoTest, Stop_Default_HandlerStopShouldBeCalled)
{
    auto video = constructVideo();

    EXPECT_CALL(adaptor(), stop());

    video->stop();
}

TEST_F(VideoTest, Width_HandlerReturnsDefaultWidth_VideoWidthEqualsDefault)
{
    auto video = constructVideo();

    ON_CALL(adaptor(), width()).WillByDefault(Return(DEFAULT_WIDTH));
    ON_CALL(adaptor(), height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(video->width(), DEFAULT_WIDTH);
    ASSERT_EQ(video->height(), DEFAULT_HEIGHT);
}

TEST_F(VideoTest, Scale_Default_VideoScaleShouldBeCalled)
{
    auto video = constructVideo();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    video->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(VideoTest, HandlerEvent_StartMediaEvent_HandlerPlayShouldBeCalled)
{
    auto video = constructVideo();

    EXPECT_CALL(adaptor(), play());

    video->handleEvent(StartMediaEvent{});
}

TEST_F(VideoTest, HandlerEvent_StopMediaEvent_HandlerStopShouldBeCalled)
{
    auto video = constructVideo();

    EXPECT_CALL(adaptor(), stop());

    video->handleEvent(StopMediaEvent{});
}

TEST_F(VideoTest, HandlerEvent_DurationExpiredMediaEvent_HandlerStopPlaybackShouldBeCalled)
{
    auto video = constructVideo();

    EXPECT_CALL(adaptor(), stopPlayback());

    video->handleEvent(DurationExpiredEvent{});
}

TEST_F(VideoTest, HandlerEvent_ScaledMediaEvent_HandlerScaleShouldBeCalled)
{
    auto video = constructVideo();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    video->handleEvent(ScaleMediaEvent{DEFAULT_XSCALE, DEFAULT_YSCALE});
}
