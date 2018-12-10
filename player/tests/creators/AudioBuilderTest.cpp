#include "AudioBuilderTest.hpp"
#include "media/Audio.hpp"

TEST(AudioBuilderTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    AudioBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), load(DEFAULT_FULL_PATH));

    builder.build();
}

TEST(AudioBuilderTest, Construct_MutedTrue_HandlerSetVolume0ShouldBeCalled)
{
    AudioBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), setVolume(MIN_VOLUME));

    builder.muted(true).build();
}

TEST(AudioBuilderTest, Construct_MutedTrue_HandlerSetVolume100ShouldBeCalled)
{
    AudioBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), setVolume(MAX_VOLUME));

    builder.volume({}).muted(false).build();
}

TEST(AudioBuilderTest, Construct_Volume50_HandlerSetVolume50ShouldBeCalled)
{
    AudioBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), setVolume(50));

    builder.volume(50).muted(false).build();
}

TEST(AudioBuilderTest, Construct_LoopTrue_AudioLoopedEqualsTrue)
{
    auto audio = AudioBuilderTest().volume({}).muted(DEFAULT_AUDIO_MUTED).looped(true).build();

    ASSERT_EQ(dynamic_cast<Audio&>(*audio).looped(), true);
}

TEST(AudioBuilderTest, Construct_LoopFalse_AudioLoopedEqualsFalse)
{
    auto audio = AudioBuilderTest().volume({}).muted(DEFAULT_AUDIO_MUTED).looped(false).build();

    ASSERT_EQ(dynamic_cast<Audio&>(*audio).looped(), false);
}


