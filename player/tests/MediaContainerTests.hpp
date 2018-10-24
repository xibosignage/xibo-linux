#pragma once

#include "test_utils.hpp"

#include "creators/MediaContainerBuilder.hpp"
#include "control/MediaContainer.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockFixedLayoutAdaptor.hpp"
#include "mocks/MockMedia.hpp"
#include "mocks/MockWidgetAdaptor.hpp"
#include "mocks/MockTimerProvider.hpp"

inline testing::NiceMock<MockVisibleMedia>* createMediaWithPos()
{
    auto handler = new testing::NiceMock<MockWidgetAdaptor>;
    auto media = new testing::NiceMock<MockVisibleMedia>(unique(handler));

    ON_CALL(*media, handler()).WillByDefault(testing::ReturnRef(*handler));

    return media;
}

inline testing::NiceMock<MockInvisibleMedia>* createMedia()
{
    auto media = new testing::NiceMock<MockInvisibleMedia>;
    return media;
}

class MediaContainerTest : public testing::Test
{
public:
    auto constructContainer()
    {
        auto container = construct<MediaContainer>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_ZORDER, DEFAULT_LOOP, unique(m_timer), unique(m_adaptor));
        addMediaToContainer(container);
        return container;
    }

protected:
    void SetUp() override
    {
        m_adaptor = new testing::NiceMock<MockFixedLayoutAdaptor>;
        m_timer = new testing::NiceMock<MockTimerProvider>;
    }

    void TearDown() override
    {
        m_adaptor = nullptr;
        m_timer = nullptr;
        m_invisibleMedia = nullptr;
        m_visibleMedia = nullptr;
    }

    MockInvisibleMedia& firstInvisibleMedia()
    {
        return *m_invisibleMedia;
    }

    MockVisibleMedia& secondVisibleMedia()
    {
        return *m_visibleMedia;
    }

    MockFixedLayoutAdaptor& adaptor()
    {
        return *m_adaptor;
    }

    MockTimerProvider& timer()
    {
        return *m_timer;
    }

private:
    void addMediaToContainer(std::shared_ptr<MediaContainer> container)
    {
        m_invisibleMedia = createMedia();
        m_visibleMedia = createMediaWithPos();

        container->addMedia(unique(m_invisibleMedia));
        container->addMedia(unique(m_visibleMedia), DEFAULT_XPOS, DEFAULT_YPOS);
    }

private:
    MockFixedLayoutAdaptor* m_adaptor = nullptr;
    MockTimerProvider* m_timer = nullptr;
    MockInvisibleMedia* m_invisibleMedia = nullptr;
    MockVisibleMedia* m_visibleMedia = nullptr;

};

