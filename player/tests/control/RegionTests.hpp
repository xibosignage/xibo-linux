#pragma once

#include "BaseTestWithHandler.hpp"

#include "creators/RegionBuilderTests.hpp" // FIXME zorder from here
#include "control/Region.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

#include "mocks/MockRegionContent.hpp"
#include "mocks/MockWidgetAdaptor.hpp"

const int DEFAULT_CONTENT_ITEMS_COUNT = 1;






#include "creators/RegionBuilderTests.hpp"

class RegionTest : public BaseTestWithHandler<MockFixedLayoutAdaptor>
{
public:
    auto constructRegion(std::size_t contentItemsCount = DEFAULT_CONTENT_ITEMS_COUNT)
    {
        RegionBuilderTest builder;
        ResourcesXlf::RegionOptions opts{DEFAULT_ID, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0, 0, DEFAULT_ZORDER, false};
        builder.adaptor(unique(&adaptor())).options(opts);
        addContentItemsToRegion(builder, contentItemsCount);
        return builder.build();
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
    void addContentItemsToRegion(RegionBuilder& builder, std::size_t contentItemsCount)
    {
        std::vector<ContentWithPos> allcontent;
        for(size_t i = 0; i != contentItemsCount; ++i)
        {
            auto content = createRegionContent();

            m_contentItems.push_back(content.get());

            allcontent.emplace_back(ContentWithPos{std::move(content), DEFAULT_XPOS, DEFAULT_YPOS});
        }
        builder.content(std::move(allcontent));
    }

private:
    std::vector<MockRegionContent*> m_contentItems;

};

