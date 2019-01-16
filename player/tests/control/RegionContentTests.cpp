#include "RegionContentTests.hpp"

using namespace testing;

TEST_F(RegionContentTest, Handler_VisibleMedia_EqualsToVisibleMediaHandler)
{
    auto content = constructRegionContent<MockVisibleMedia>();
    auto&& visibleMedia = dynamic_cast<MockVisibleMedia&>(media());

    ASSERT_EQ(&content->handler(), &visibleMedia.handler());
}

TEST_F(RegionContentTest, Handler_InvisibleMedia_EqualsToDefaultBlankHandler)
{
    auto content = constructRegionContent();

    ASSERT_EQ(&content->handler(), &content->blankHandler());
}

TEST_F(RegionContentTest, Start_MediaDurationGreaterThan0_TimerStartOnceWithMediaDuration)
{
    auto content = constructRegionContent();

    ON_CALL(media(), duration()).WillByDefault(Return(DEFAULT_DURATION));
    EXPECT_CALL(timer(), startOnce(DEFAULT_DURATION * MSECS, _));

    content->start();
}

TEST_F(RegionContentTest, Start_MediaDurationEquals0_TimerStartOnceShouldNotBeCalled)
{
    auto content = constructRegionContent();

    EXPECT_CALL(timer(), startOnce(_, _)).Times(0);

    content->start();
}

//TEST_F(RegionContentTest, Start_Default_MediaHandleStartMediaEventShouldBeCalled)
//{
//    auto content = constructRegionContent();

//    EXPECT_CALL(media(), handleEvent(ResultOf(eventHandler(), EventType::StartMedia)));

//    content->start();
//}

//TEST_F(RegionContentTest, Start_WithAttachedMedia_AttachedMediaHandleStartMediaEventShouldBeCalled)
//{
//    auto attachedMedia = createMedia<MockMedia>();
//    auto content = constructRegionContent();

//    EXPECT_CALL(*attachedMedia, handleEvent(ResultOf(eventHandler(), EventType::StartMedia)));

//    content->attachMedia(unique(attachedMedia));
//    content->start();
//}

//TEST_F(RegionContentTest, Stop_WithAttachedMedia_AttachedMediaHandleStopMediaEventShouldBeCalled)
//{
//    auto attachedMedia = createMedia<MockMedia>();
//    auto content = constructRegionContent();

//    EXPECT_CALL(*attachedMedia, handleEvent(ResultOf(eventHandler(), EventType::StopMedia)));

//    content->attachMedia(unique(attachedMedia));
//    content->stop();
//}

//TEST_F(RegionContentTest, Stop_Default_MediaHandleStopMediaEventShouldBeCalled)
//{
//    auto content = constructRegionContent();

//    EXPECT_CALL(media(), handleEvent(ResultOf(eventHandler(), EventType::StopMedia)));

//    content->stop();
//}

//TEST_F(RegionContentTest, Scale_InvisibleMedia_MediaHandleScaleMediaEventShouldNotBeCalled)
//{
//    auto content = constructRegionContent();

//    EXPECT_CALL(media(), handleEvent(_)).Times(0);

//    content->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
//}

//TEST_F(RegionContentTest, Scale_VisibleMedia_MediaHandleScaleMediaEventShouldBeCalled)
//{
//    auto content = constructRegionContent<MockVisibleMedia>();

//    EXPECT_CALL(media(), handleEvent(ResultOf(eventHandler(), EventType::ScaleMedia)));

//    content->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
//}
