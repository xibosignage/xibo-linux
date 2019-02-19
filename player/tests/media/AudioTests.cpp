#include "AudioTests.hpp"

using namespace testing;

TEST_F(AudioTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), load(DEFAULT_FULL_PATH));

    constructAudio();
}

TEST_F(AudioTest, Construct_MutedTrue_HandlerSetVolume0ShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setVolume(MIN_VOLUME));

    constructAudio(true, DEFAULT_AUDIO_LOOPED, MAX_VOLUME);
}

TEST_F(AudioTest, Construct_MutedFalseNoVolume_HandlerSetVolume100ShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setVolume(MAX_VOLUME));

    constructAudio(false, DEFAULT_AUDIO_LOOPED, {});
}

TEST_F(AudioTest, Construct_Volume50_HandlerSetVolume50ShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setVolume(50));

    constructAudio(DEFAULT_AUDIO_MUTED, DEFAULT_AUDIO_LOOPED, 50);
}

TEST_F(AudioTest, Construct_LoopTrue_AudioLoopedEqualsTrue)
{
    auto audio = constructAudio(DEFAULT_AUDIO_MUTED, true, MAX_VOLUME);

    ASSERT_EQ(audio->looped(), true);
}

TEST_F(AudioTest, Construct_LoopFalse_AudioLoopedEqualsFalse)
{
    auto audio = constructAudio(DEFAULT_AUDIO_MUTED, false, MAX_VOLUME);

    ASSERT_EQ(audio->looped(), false);
}

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
