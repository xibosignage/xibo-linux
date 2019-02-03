#pragma once

#include "test_utils.hpp"

#include "control/RegionContent.hpp"
#include "mocks/MockTimerProvider.hpp"

#include "mocks/MockWidgetAdaptor.hpp"
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
    RegionContentTest()
    {
        m_timer = new testing::NiceMock<MockTimerProvider>();
    }

    template<typename Media = MockMedia>
    auto constructRegionContent()
    {
        m_media = createMedia<Media>();

        return construct<RegionContentUnderTest>(unique(m_media), unique(m_timer));
    }

protected:
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

    template<typename Media>
    auto createMedia()
    {
        if constexpr(std::is_same_v<Media, MockVisibleMedia>)
            return new MockVisibleMedia(std::make_unique<MockWidgetAdaptor>());
        else
            return new MockMedia;
    }

private:
    MockTimerProvider* m_timer = nullptr;
    MockMedia* m_media = nullptr;

};

