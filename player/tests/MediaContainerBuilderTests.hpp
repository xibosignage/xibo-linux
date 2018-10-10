#pragma once

#include "test_utils.hpp"

#include "creators/MediaContainerBuilder.hpp"

#include "mocks/MockMedia.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"
#include "mocks/MockMediaContainer.hpp"
#include "mocks/MockWidgetAdaptor.hpp"
#include "media/MediaVisitor.hpp"

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

    MediaContainerBuilderTest& defaultVisibleMedia()
    {
        std::vector<MediaStruct> allMedia;
        allMedia.push_back(MediaStruct{fake_construct<MockMedia, MockWidgetAdaptor>(), "video", DEFAULT_X_POS, DEFAULT_Y_POS});
        return static_cast<MediaContainerBuilderTest&>(media(std::move(allMedia)));
    }

    MediaContainerBuilderTest& defaultInvisibleMedia() // FIXME positions should not be there
    {
        std::vector<MediaStruct> allMedia;
        allMedia.push_back(MediaStruct{fake_construct<MockMedia, MockWidgetAdaptor>(), "audio", DEFAULT_X_POS, DEFAULT_Y_POS});
        return static_cast<MediaContainerBuilderTest&>(media(std::move(allMedia)));
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
