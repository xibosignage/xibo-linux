#include "VideoTests.hpp"
#include "adaptors/IWidgetAdaptor.hpp"

using namespace testing;

TEST_F(VideoTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructVideo();
}

TEST_F(VideoTest, Construct_Default_HandlerLoadShouldBeCalled)
{
    EXPECT_CALL(adaptor(), load(DEFAULT_PATH));

    constructVideo();
}

//TEST_F(VideoTest, Start_Default_HandlerShowShouldBeCalled)
//{
//    auto video = constructVideo();

//    EXPECT_CALL(adaptor(), play());

//    video->start();
//}

//TEST_F(VideoTest, Stop_Default_HandlerHideShouldBeCalled)
//{
//    auto video = constructVideo();

//    EXPECT_CALL(adaptor(), stop());

//    video->stop();
//}

TEST_F(VideoTest, SetMuted_True_HandlerSetVolume0ShouldBeCalled)
{
    auto video = constructVideo();

    EXPECT_CALL(adaptor(), setVolume(MIN_VOLUME));

    video->setMuted(true);
}

TEST_F(VideoTest, SetMuted_False_HandlerSetVolume100ShouldBeCalled)
{
    auto video = constructVideo();

    EXPECT_CALL(adaptor(), setVolume(MAX_VOLUME));

    video->setMuted(false);
}

TEST_F(VideoTest, Width_HandlerReturnsDefaultWidth_VideoWidthEqualsDefault)
{
    auto video = constructVideo();

    ON_CALL(adaptor(), width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(video->width(), DEFAULT_WIDTH);
}

TEST_F(VideoTest, Height_HandlerReturnsDefaultHeight_VideoHeightEqualsDefault)
{
    auto video = constructVideo();

    ON_CALL(adaptor(), height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(video->height(), DEFAULT_HEIGHT);
}

TEST_F(VideoTest, Scale_Default_VideoScaleShouldBeCalled)
{
    auto video = constructVideo();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    video->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}
