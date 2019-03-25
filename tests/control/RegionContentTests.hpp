#pragma once

#include "test_utils.hpp"

#include "control/RegionContent.hpp"
#include "mocks/MockTimerProvider.hpp"

#include "mocks/MockWidget.hpp"
#include "mocks/MockMedia.hpp"

const int DEFAULT_CONTENT_ITEMS_COUNT = 1;

class RegionContentUnderTest : public RegionContent
{
public:
    using RegionContent::RegionContent;

    IWidgetAdaptor& blankHandler() override
    {
        static MockWidgetAdaptor handler;
        return handler;
    }

};

class RegionContentTest : public testing::Test
{
public:
    template<typename Media = MockMedia>
    auto constructRegionContent()
    {
        auto media = constructMock<Media>();
        m_media = media.get();

        return construct<RegionContentUnderTest>(std::move(media), unique(m_timer));
    }

protected:
    void SetUp() override
    {
        m_timer = new testing::NiceMock<MockTimerProvider>();
    }

    MockTimerProvider& timer()
    {
        return *m_timer;
    }

    MockMedia& media()
    {
        return *m_media;
    }

    auto eventHandler()
    {
        return [](const Event& ev) { return ev.type(); };
    }

private:
    MockTimerProvider* m_timer = nullptr;
    MockMedia* m_media = nullptr;

};

