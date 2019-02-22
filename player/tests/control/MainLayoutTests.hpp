#pragma once

#include "BaseTestWithHandler.hpp"

#include "creators/MainLayoutBuilder.hpp"
#include "control/MainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"
#include "parsers/LayoutOptions.hpp"

#include "creators/BackgroundBuilder.hpp"
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
const int DEFAULT_REGIONS_COUNT = 3;
const LayoutOptions DEFAULT_LAYOUT_OPTIONS{DEFAULT_SCHEME, DEFAULT_WIDTH, DEFAULT_HEIGHT};

class MainLayoutTest : public BaseTestWithHandler<MockOverlayAdaptor>
{
public:
    auto constructLayout(size_t regionsCount = DEFAULT_REGIONS_COUNT)
    {
        auto background = createBackground();
        auto regions = createRegions(regionsCount);

        m_background = background.get();

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

    MockRegion& region(size_t index = 0)
    {
        return *m_regions.at(index);
    }

    std::unique_ptr<MockBackground> createBackground()
    {
        return constructMock<MockBackground>();
    }

    auto createRegion()
    {
        return constructMock<MockRegion>();
    }

    virtual std::vector<RegionWithPos> createRegions(size_t regionsCount)
    {
        std::vector<RegionWithPos> regions;
        for(size_t i = 0; i != regionsCount; ++i)
        {
            auto region = createRegion();

            m_regions.push_back(region.get());

            regions.emplace_back(RegionWithPos{std::move(region), DEFAULT_XPOS, DEFAULT_YPOS});
        }
        return regions;
    }

    MockBackground* m_background = nullptr;
    std::vector<MockRegion*> m_regions;

};

class MainLayoutReorderTest : public MainLayoutTest, public testing::WithParamInterface<std::vector<int>>
{
protected:
    std::vector<RegionWithPos> createRegions(size_t) override
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

private:
    void pushRegionAndSort(testing::NiceMock<MockRegion>* region)
    {
        m_regions.push_back(region);
        std::sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
            return first->zorder() < second->zorder();
        });
    }

};

class MainLayoutConstructSizeTest : public MainLayoutTest, public testing::WithParamInterface<Size> { };
