#pragma once

#include "BaseTestWithHandler.hpp"

#include "creators/MediaContainerBuilderTests.hpp"
#include "control/MediaContainer.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"
#include "mocks/MockTimerProvider.hpp"

#include "mocks/MockMedia.hpp"
#include "mocks/MockWidgetAdaptor.hpp"
#include "media/MediaVisitor.hpp"

class MediaContainerTest : public BaseTestWithHandler<MockFixedLayoutAdaptor>
{
public:
    auto constructContainer()
    {
        auto container = construct<MediaContainer>(DEFAULT_ID, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_ZORDER, unique(m_timer), unique(&adaptor()));
        addMediaToContainer(*container);
        return container;
    }

protected:
    void doSetUp() override
    {
        m_timer = new testing::NiceMock<MockTimerProvider>;
    }

    MockInvisibleMedia& firstInvisibleMedia()
    {
        return *m_invisibleMedia;
    }

    MockVisibleMedia& secondVisibleMedia()
    {
        return *m_visibleMedia;
    }
    MockTimerProvider& timer()
    {
        return *m_timer;
    }

    auto createMediaWithPos()
    {
        return constructMock<MockVisibleMedia, MockWidgetAdaptor>();
    }

    auto createMedia()
    {
        return constructMock<MockInvisibleMedia>();
    }

private:
    void addMediaToContainer(MediaContainer& container)
    {
        auto visibleMedia = createMediaWithPos();
        auto invisibleMedia = createMedia();

        m_visibleMedia = visibleMedia.get();
        m_invisibleMedia = invisibleMedia.get();

        container.addMedia(std::move(invisibleMedia));
        container.addMedia(std::move(visibleMedia), DEFAULT_XPOS, DEFAULT_YPOS);
    }

private:
    MockTimerProvider* m_timer = nullptr;
    MockInvisibleMedia* m_invisibleMedia = nullptr;
    MockVisibleMedia* m_visibleMedia = nullptr;

};

