#pragma once

#include "creators/MediaBuilderTest.hpp"
#include "creators/AudioBuilder.hpp"
#include "mocks/MockAudioHandler.hpp"

class AudioBuilderTest : public MediaBuilderTest<AudioBuilder>
{
public:
    AudioBuilderTest()
    {
        m_adaptor = std::make_unique<testing::NiceMock<MockAudioHandler>>();

        ON_CALL(filesystem(), isRegularFile(testing::_)).WillByDefault(testing::Return(true));;
        path(DEFAULT_PATH.string()).duration(DEFAULT_DURATION);
    }

    testing::NiceMock<MockAudioHandler>& adaptor()
    {
        return *m_adaptor;
    }

protected:
    std::unique_ptr<IAudioHandler> createHandler() override
    {
        return std::move(m_adaptor);
    }

private:
    std::unique_ptr<testing::NiceMock<MockAudioHandler>> m_adaptor;

};
