#include "AudioBuilderTest.hpp"
#include "media/IMedia.hpp"

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

