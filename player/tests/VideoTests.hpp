#pragma once

#include "test_utils.hpp"

#include "media/Video.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockVideoHandler.hpp"

class VideoTest : public testing::Test
{
public:
    auto constructVideo()
    {
        auto video = construct<Video>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, unique(m_adaptor));
        video->setDuration(DEFAULT_DURATION);
        return video;
    }

protected:
    void SetUp() override
    {
        m_adaptor = new testing::NiceMock<MockVideoHandler>;
    }

    void TearDown() override
    {
        m_adaptor = nullptr;
    }

    MockVideoHandler& adaptor()
    {
        return *m_adaptor;
    }

private:
    MockVideoHandler* m_adaptor = nullptr;

};
