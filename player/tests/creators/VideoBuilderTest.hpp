#pragma once

#include "creators/MediaBuilderTest.hpp"
#include "creators/VideoBuilder.hpp"
#include "mocks/MockVideoHandler.hpp"

class VideoBuilderTest : public MediaBuilderTest<VideoBuilder>
{
public:
    VideoBuilderTest()
    {
        m_adaptor = std::make_unique<testing::NiceMock<MockVideoHandler>>();

        ON_CALL(filesystem(), isRegularFile(testing::_)).WillByDefault(testing::Return(true));
    }

    VideoBuilderTest& adaptor(std::unique_ptr<testing::NiceMock<MockVideoHandler>> adaptor)
    {
        m_adaptor = std::move(adaptor);
        return *this;
    }

protected:
    std::unique_ptr<IVideoHandler> createHandler() override
    {
        return std::move(m_adaptor);
    }

private:
    std::unique_ptr<testing::NiceMock<MockVideoHandler>> m_adaptor;

};
