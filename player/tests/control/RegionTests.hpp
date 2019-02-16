#pragma once

#include "BaseTestWithHandler.hpp"

#include "creators/RegionBuilderTests.hpp" // FIXME zorder from here
#include "control/Region.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

#include "mocks/MockRegionContent.hpp"
#include "mocks/MockWidgetAdaptor.hpp"
#include "media/MediaVisitor.hpp"

const int DEFAULT_CONTENT_ITEMS_COUNT = 1;






#include "creators/RegionBuilder.hpp"

class RegionTest : public BaseTestWithHandler<MockFixedLayoutAdaptor>
{
public:
    auto constructRegion(std::size_t contentItemsCount = DEFAULT_CONTENT_ITEMS_COUNT)
    {
        auto region = construct<Region>(DEFAULT_ID, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_ZORDER, unique(&adaptor()));
        addContentItemsToRegion(*region, contentItemsCount);
        return region;
    }

protected:
    MockRegionContent& regionContent(size_t index)
    {
        return *m_contentItems.at(index);
    }

    auto createRegionContent()
    {
        return constructMock<MockRegionContent, MockWidgetAdaptor>();
    }

private:
    void addContentItemsToRegion(Region& region, std::size_t contentItemsCount)
    {
        for(size_t i = 0; i != contentItemsCount; ++i)
        {
            auto content = createRegionContent();

            m_contentItems.push_back(content.get());

            region.addContent(std::move(content), DEFAULT_XPOS, DEFAULT_YPOS);
        }
    }

private:
    std::vector<MockRegionContent*> m_contentItems;

};

