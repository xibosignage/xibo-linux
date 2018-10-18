#pragma once

#include "test_utils.hpp"

#include "creators/MediaContainerBuilder.hpp"

#include "mocks/MockMedia.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"
#include "mocks/MockMediaContainer.hpp"
#include "mocks/MockWidgetAdaptor.hpp"
#include "media/MediaVisitor.hpp"
#include "mocks/MockTimerProvider.hpp"

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

const auto invalidMediaContainerSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_WIDTH, MAX_DISPLAY_HEIGHT, MIN_HEIGHT>;

class MediaContainerBuilderSizeTest : public testing::TestWithParam<Size> { };

class MediaContainerBuilderTest : public MediaContainerBuilder
{
public:
    MediaContainerBuilderTest& fakeContainer(std::unique_ptr<IMediaContainer>&& container)
    {
        m_container = std::move(container);
        return static_cast<MediaContainerBuilderTest&>(*this);
    }

    MediaContainerBuilderTest& defaultAdaptor()
    {
        return static_cast<MediaContainerBuilderTest&>(adaptor(std::make_unique<testing::NiceMock<MockFixedLayoutAdaptor>>()));
    }

    MediaContainerBuilderTest& defaultTimer()
    {
        return static_cast<MediaContainerBuilderTest&>(timer(std::make_unique<testing::NiceMock<MockTimerProvider>>()));
    }

    MediaContainerBuilderTest& defaultVisibleMedia()
    {
        std::vector<MediaWithPos> media;
        media.push_back(MediaWithPos{fake_construct<MockVisibleMedia, MockWidgetAdaptor>(), DEFAULT_XPOS, DEFAULT_YPOS});
        return static_cast<MediaContainerBuilderTest&>(visibleMedia(std::move(media)));
    }

    MediaContainerBuilderTest& defaultInvisibleMedia()
    {
        std::vector<std::unique_ptr<IMedia>> media;
        media.push_back(std::make_unique<MockInvisibleMedia>());
        return static_cast<MediaContainerBuilderTest&>(invisibleMedia(std::move(media)));
    }

    MediaContainerBuilder& defaultSize()
    {
        return width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT);
    }

protected:
    std::unique_ptr<IMediaContainer> createContainer(int zorder, bool) override
    {
        if(m_container)
            return std::move(m_container);

        auto container = fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>();
        ON_CALL(*container, zorder()).WillByDefault(testing::Return(zorder));
        return container;
    }

private:
    std::unique_ptr<IMediaContainer> m_container;

};
