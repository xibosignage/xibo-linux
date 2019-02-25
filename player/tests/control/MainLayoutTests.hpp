#pragma once

#include "BaseTestWithHandler.hpp"

#include "creators/MainLayoutBuilder.hpp"
#include "control/MainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"
#include "options/LayoutOptions.hpp"

#include "mocks/MockBackground.hpp"
#include "mocks/MockRegion.hpp"

const int DEFAULT_REGIONS_COUNT = 3;

class MainLayoutTest : public BaseTestWithHandler<MockOverlayAdaptor>
{
public:
    auto constructLayout(size_t regionsCount = DEFAULT_REGIONS_COUNT)
    {
        const LayoutOptions opts{DEFAULT_WIDTH, DEFAULT_HEIGHT};
        auto background = createBackground();
        auto regions = createRegions(regionsCount);

        m_background = background.get();

        return MainLayoutBuilder{}.adaptor(unique(&adaptor()))
                                  .regions(std::move(regions))
                                  .background(std::move(background))
                                  .options(opts)
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

    std::unique_ptr<MockRegion> createRegion()
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
    void pushRegionAndSort(MockRegion* region)
    {
        m_regions.push_back(region);
        std::sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
            return first->zorder() < second->zorder();
        });
    }

};

class MainLayoutSizeTest : public MainLayoutTest, public testing::WithParamInterface<Size> { };
