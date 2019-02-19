#pragma once

#include "BaseTestWithHandler.hpp"

#include "creators/MainLayoutBuilder.hpp"
#include "control/MainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"
#include "parsers/LayoutOptions.hpp"

#include "mocks/MockBackground.hpp"
#include "mocks/MockImageAdaptor.hpp"

#include "mocks/MockRegion.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

const std::vector<std::vector<int>> zorders = {
    {10,9,8},
    {1,2,4,10},
    {10,7,9,1},
    {5,4},
    {1},
    {4,1,5,2},
    {4,3,2,1,0}
};

const int DEFAULT_SCHEME = 5;
const ResourcesXlf::LayoutOptions DEFAULT_LAYOUT_OPTIONS{DEFAULT_SCHEME, DEFAULT_WIDTH, DEFAULT_HEIGHT};

class MainLayoutTest : public BaseTestWithHandler<MockOverlayAdaptor>
{
public:
    auto constructLayout()
    {
        auto background = createBackground();
        auto regions = createRegions();

        m_background = background.get();
        m_region = dynamic_cast<MockRegion*>(regions.back().region.get());

        return MainLayoutBuilder{}.adaptor(unique(&adaptor()))
                                  .regions(std::move(regions))
                                  .background(std::move(background))
                                  .options(DEFAULT_LAYOUT_OPTIONS)
                                  .build();
    }

protected:
    void doSetUp() override
    {
        ON_CALL(adaptor(), width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(adaptor(), height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));
    }

    MockBackground& background()
    {
        return *m_background;
    }

    MockRegion& region()
    {
        return *m_region;
    }

    virtual std::unique_ptr<MockBackground> createBackground()
    {
        auto background = constructMock<MockBackground, MockImageAdaptor>();

        ON_CALL(*background, width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(*background, height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));

        return background;
    }

    auto createRegion()
    {
        auto region = constructMock<MockRegion, MockFixedLayoutAdaptor>();

        ON_CALL(*region, width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(*region, height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));

        return region;
    }

    virtual std::vector<RegionWithPos> createRegions()
    {
        std::vector<RegionWithPos> regions;
        regions.emplace_back(RegionWithPos{createRegion(), DEFAULT_XPOS, DEFAULT_XPOS});
        return regions;
    }

private:
    MockBackground* m_background = nullptr;
    MockRegion* m_region = nullptr;

};

class MainLayoutReorderTest : public MainLayoutTest, public testing::WithParamInterface<std::vector<int>>
{
protected:
    std::vector<RegionWithPos> createRegions() override
    {
        std::vector<RegionWithPos> regions;
        for(int zorder : GetParam())
        {
            auto region = createRegion();

            ON_CALL(*region, zorder()).WillByDefault(testing::Return(zorder));
            pushRegionAndSort(region.get());

            regions.emplace_back(RegionWithPos{std::move(region), DEFAULT_XPOS, DEFAULT_YPOS});
        }
        return regions;
    }

    testing::NiceMock<MockRegion>* region(size_t index)
    {
        return m_regions[index];
    }

private:
    void pushRegionAndSort(testing::NiceMock<MockRegion>* region)
    {
        m_regions.push_back(region);
        std::sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
            return first->zorder() < second->zorder();
        });
    }

private:
    std::vector<testing::NiceMock<MockRegion>*> m_regions;

};

class MainLayoutConstructSizeTest : public MainLayoutTest, public testing::WithParamInterface<Size> { };
