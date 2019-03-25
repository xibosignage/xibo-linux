#pragma once

#include "BaseTestWithHandler.hpp"

#include "creators/RegionBuilder.hpp"
#include "control/Region.hpp"
#include "mocks/MockFixedLayout.hpp"

#include "mocks/MockRegionContent.hpp"
#include "mocks/MockWidget.hpp"

const int DEFAULT_CONTENT_ITEMS_COUNT = 1;

class RegionTest : public BaseTestWithHandler<MockFixedLayoutAdaptor>
{
public:
    std::unique_ptr<IRegion> constructRegion(boost::optional<RegionOptions::Loop> contentLooped,
                                             boost::optional<int> zorder,
                                             std::size_t contentItemsCount = DEFAULT_CONTENT_ITEMS_COUNT)
    {
        const RegionOptions opts{DEFAULT_ID, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_XPOS, DEFAULT_YPOS, zorder, contentLooped};

        return RegionBuilder{}.adaptor(unique(&adaptor()))
                              .content(createContentItems(contentItemsCount))
                              .options(opts)
                              .build();
    }

    std::unique_ptr<IRegion> constructRegion(std::size_t contentItemsCount = DEFAULT_CONTENT_ITEMS_COUNT)
    {
        return constructRegion(DEFAULT_REGION_LOOP, DEFAULT_REGION_ZORDER, contentItemsCount);
    }

protected:
    void doSetUp() override
    {
        ON_CALL(adaptor(), width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(adaptor(), height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));
    }

    MockRegionContent& regionContent(size_t index = 0)
    {
        return *m_contentItems.at(index);
    }

    std::unique_ptr<MockRegionContent> createRegionContent()
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
