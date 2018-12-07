#include "AudioTests.hpp"

using namespace testing;

const int DEFAULT_VOLUME = 100;

TEST_F(AudioTest, Construct_Default_HandlerLoadShouldBeCalled)
{
    EXPECT_CALL(adaptor(), load(DEFAULT_PATH));

    constructAudio();
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

TEST_F(AudioTest, SetVolume_Default_HandlerSetVolumeShouldBeCalled)
{
    auto audio = constructAudio();

    EXPECT_CALL(adaptor(), setVolume(DEFAULT_VOLUME));

    audio->setVolume(DEFAULT_VOLUME);
}
