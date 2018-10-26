#pragma once

#include "test_utils.hpp"

#include "creators/MediaContainerBuilder.hpp"

#include "mocks/MockMedia.hpp"
#include "mocks/MockWidgetAdaptor.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockFixedLayoutAdaptor.hpp"
#include "mocks/MockTimerProvider.hpp"

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

const auto invalidMediaContainerSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_WIDTH, MAX_DISPLAY_HEIGHT, MIN_HEIGHT>;

class MediaContainerBuilderSizeTest : public testing::TestWithParam<Size> { };

class MediaContainerBuilderTest : public MediaContainerBuilder
{
public:
    MediaContainerBuilderTest& adaptor(std::unique_ptr<testing::NiceMock<MockFixedLayoutAdaptor>>&& adaptor)
    {
        m_adaptor = std::move(adaptor);
        return static_cast<MediaContainerBuilderTest&>(*this);
    }

    MediaContainerBuilderTest& defaultVisibleMedia()
    {
        std::vector<MediaWithPos> media;
        media.push_back(MediaWithPos{constructMock<MockVisibleMedia, MockWidgetAdaptor>(), DEFAULT_XPOS, DEFAULT_YPOS});
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
    std::unique_ptr<IFixedLayoutAdaptor> createAdaptor() override
    {
        if(m_adaptor)
            return std::move(m_adaptor);

        return std::make_unique<testing::NiceMock<MockFixedLayoutAdaptor>>();
    }

    std::unique_ptr<ITimerProvider> createTimer() override
    {
        return std::make_unique<testing::NiceMock<MockTimerProvider>>();
    }

private:
    std::unique_ptr<testing::NiceMock<MockFixedLayoutAdaptor>> m_adaptor;

};
