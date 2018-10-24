#pragma once

#include "test_utils.hpp"

#include "media/Audio.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockAudioHandler.hpp"

class AudioTest : public testing::Test
{
public:
    auto constructAudio()
    {
        auto audio = construct<Audio>(DEFAULT_PATH, unique(m_adaptor));
        audio->setDuration(DEFAULT_DURATION);
        return audio;
    }

protected:
    void SetUp() override
    {
        m_adaptor = new testing::NiceMock<MockAudioHandler>;
    }

    void TearDown() override
    {
        m_adaptor = nullptr;
    }

    MockAudioHandler& adaptor()
    {
        return *m_adaptor;
    }

private:
    MockAudioHandler* m_adaptor = nullptr;

};

