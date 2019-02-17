#include "AudioTests.hpp"

using namespace testing;

//TEST_F(AudioTest, Construct_Default_HandlerLoadShouldBeCalled)
//{
//    EXPECT_CALL(adaptor(), load(DEFAULT_PATH));

//    constructAudio();
//}

TEST_F(AudioTest, Play_Default_HandlerPlayShouldBeCalled)
{
    auto audio = constructAudio();

    EXPECT_CALL(adaptor(), play());

    audio->play();
}

TEST_F(AudioTest, Stop_Default_HandlerStopShouldBeCalled)
{
    auto audio = constructAudio();

    EXPECT_CALL(adaptor(), stop());

    audio->stop();
}

TEST_F(AudioTest, SetVolume_Default_HandlerSetVolumeShouldBeCalled)
{
    auto audio = constructAudio();

    EXPECT_CALL(adaptor(), setVolume(DEFAULT_VOLUME));

    audio->setVolume(DEFAULT_VOLUME);
}

TEST_F(AudioTest, SetLooped_True_LoopedEqualsTrue)
{
    auto audio = constructAudio();

    audio->setLooped(true);

    ASSERT_EQ(audio->looped(), true);
}

TEST_F(AudioTest, SetLooped_False_LoopedEqualsFalse)
{
    auto audio = constructAudio();

    audio->setLooped(false);

    ASSERT_EQ(audio->looped(), false);
}

TEST_F(AudioTest, HandlerEvent_StartMediaEvent_HandlerPlayShouldBeCalled)
{
    auto audio = constructAudio();

    EXPECT_CALL(adaptor(), play());

    audio->handleEvent(StartMediaEvent{});
}

TEST_F(AudioTest, HandlerEvent_StopMediaEvent_HandlerStopShouldBeCalled)
{
    auto audio = constructAudio();

    EXPECT_CALL(adaptor(), stop());

    audio->handleEvent(StopMediaEvent{});
}

TEST_F(AudioTest, HandlerEvent_DurationExpiredMediaEvent_HandlerStopShouldBeCalled)
{
    auto audio = constructAudio();

    EXPECT_CALL(adaptor(), stop());

    audio->handleEvent(DurationExpiredEvent{});
}
