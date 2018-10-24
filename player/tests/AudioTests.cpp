#include "AudioTests.hpp"

using namespace testing;

const int DEFAULT_VOLUME = 100;

TEST(AudioTest, Construct_Default_HandlerLoadShouldBeCalled)
{
    auto audioHandlerMock = std::make_unique<NiceMock<MockAudioHandler>>();

    EXPECT_CALL(*audioHandlerMock, load(DEFAULT_PATH));

    constructAudio(std::move(audioHandlerMock));
}

TEST(AudioTest, Start_Default_HandlerShowShouldBeCalled)
{
    auto [audio, audioHandlerMock] = constructAudio();

    EXPECT_CALL(*audioHandlerMock, play());

    audio->start();
}

TEST(AudioTest, Stop_Default_HandlerHideShouldBeCalled)
{
    auto [audio, audioHandlerMock] = constructAudio();

    EXPECT_CALL(*audioHandlerMock, stop());

    audio->stop();
}

TEST(AudioTest, SetVolume_Default_HandlerSetVolumeShouldBeCalled)
{
    auto [audio, audioHandlerMock] = constructAudio();

    EXPECT_CALL(*audioHandlerMock, setVolume(DEFAULT_VOLUME));

    audio->setVolume(DEFAULT_VOLUME);
}
