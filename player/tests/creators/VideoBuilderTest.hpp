#pragma once

#include "creators/MediaBuilderTest.hpp"
#include "creators/VideoBuilder.hpp"
#include "media/MediaVisitor.hpp"
#include "mocks/MockVideoHandler.hpp"

class VideoBuilderTest : public MediaBuilderTest<VideoBuilder>
{
public:
    VideoBuilderTest()
    {
        m_adaptor = std::make_unique<testing::NiceMock<MockVideoHandler>>();

        ON_CALL(filesystem(), isRegularFile(testing::_)).WillByDefault(testing::Return(true));
        width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT).path(DEFAULT_PATH.string()).duration(DEFAULT_DURATION);
    }

    testing::NiceMock<MockVideoHandler>& adaptor()
    {
        return *m_adaptor;
    }

protected:
    std::unique_ptr<IVideoHandler> createHandler() override
    {
        return std::move(m_adaptor);
    }

private:
    std::unique_ptr<testing::NiceMock<MockVideoHandler>> m_adaptor;

};
