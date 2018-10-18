#pragma once

#include "test_utils.hpp"

#include "creators/MediaContainerBuilder.hpp"
#include "control/MediaContainer.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockFixedLayoutAdaptor.hpp"
#include "mocks/MockMedia.hpp"
#include "mocks/MockWidgetAdaptor.hpp"
#include "mocks/MockTimerProvider.hpp"

const unsigned int DEFAULT_DURATION = 10;

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

inline auto constructContainer()
{
    auto [container, handler] = construct<MediaContainer, MockFixedLayoutAdaptor>(DEFAULT_ZORDER, DEFAULT_LOOP, std::make_unique<MockTimerProvider>());
    container->addMedia(unique(createMedia()));
    return std::pair{container, handler};
}

inline auto constructContainerWithoutMedia()
{
    return construct<MediaContainer, MockFixedLayoutAdaptor>(DEFAULT_ZORDER, DEFAULT_LOOP, std::make_unique<MockTimerProvider>());
}

inline auto constructContainerWithoutMedia(std::unique_ptr<ITimerProvider>&& timer)
{
    return construct<MediaContainer, MockFixedLayoutAdaptor>(DEFAULT_ZORDER, DEFAULT_LOOP, std::move(timer));
}
