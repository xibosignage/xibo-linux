#pragma once

#include "BaseTestWithHandler.hpp"

#include "creators/RegionBuilder.hpp" // FIXME zorder from here
#include "control/Region.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

#include "mocks/MockRegionContent.hpp"
#include "mocks/MockWidgetAdaptor.hpp"

const int DEFAULT_CONTENT_ITEMS_COUNT = 1;
const RegionOptions DEFAULT_REGION_OPTIONS{DEFAULT_ID, DEFAULT_WIDTH, DEFAULT_HEIGHT,
                                                         DEFAULT_XPOS, DEFAULT_YPOS, DEFAULT_ZORDER, DEFAULT_LOOP};

class RegionTest : public BaseTestWithHandler<MockFixedLayoutAdaptor>
{
public:
    auto constructRegion(std::size_t contentItemsCount = DEFAULT_CONTENT_ITEMS_COUNT)
    {
        return RegionBuilder{}.adaptor(unique(&adaptor()))
                              .content(createContentItems(contentItemsCount))
                              .options(DEFAULT_REGION_OPTIONS)
                              .build();
    }

protected:
    MockRegionContent& regionContent(size_t index)
    {
        return *m_contentItems.at(index);
    }

    auto createRegionContent()
    {
        return constructMock<MockRegionContent>();
    }

    std::vector<ContentWithPos> createContentItems(std::size_t contentItemsCount)
    {
        std::vector<ContentWithPos> allContent;

        for(size_t i = 0; i != contentItemsCount; ++i)
        {
            auto content = createRegionContent();

            m_contentItems.push_back(content.get());

            allContent.emplace_back(ContentWithPos{std::move(content), DEFAULT_XPOS, DEFAULT_YPOS});
        }

        return allContent;
    }

private:
    std::vector<MockRegionContent*> m_contentItems;

};

class RegionConstructSizeTest : public RegionTest, public testing::WithParamInterface<Size> { };
