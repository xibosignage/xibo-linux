#pragma once

#include "BaseTestWithHandler.hpp"

#include "control/MainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"

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

#include "creators/MainLayoutBuilderTests.hpp"

class MainLayoutTest : public BaseTestWithHandler<MockOverlayAdaptor>
{
public:
    auto constructLayout()
    {
        MainLayoutBuilderTest builder;
        builder.adaptor(unique(&adaptor())).options(ResourcesXlf::LayoutOptions{5, DEFAULT_WIDTH, DEFAULT_HEIGHT});

        ON_CALL(adaptor(), width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(adaptor(), height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));

        addBackgroundAndRegions(builder);

        return builder.build();
    }

protected:
    MockBackground& background()
    {
        return *m_background;
    }

    MockRegion& region()
    {
        return *m_region;
    }

    auto createBackground()
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

    virtual void addBackground(MainLayoutBuilder& builder)
    {
        auto background = createBackground();
        m_background = background.get();

        builder.background(std::move(background));
    }

    virtual void addRegions(MainLayoutBuilder& builder)
    {
        auto region = createRegion();
        m_region = region.get();

        std::vector<RegionWithPos> regions;
        regions.emplace_back(RegionWithPos{std::move(region), DEFAULT_XPOS, DEFAULT_XPOS});
        builder.regions(std::move(regions));
    }

private:
    void addBackgroundAndRegions(MainLayoutBuilder& builder)
    {
        addBackground(builder);
        addRegions(builder);
    }

private:
    MockBackground* m_background = nullptr;
    MockRegion* m_region = nullptr;

};

class MainLayoutReorderTest : public MainLayoutTest, public testing::WithParamInterface<std::vector<int>>
{
protected:
    void addRegions(MainLayoutBuilder& builder) override
    {
        std::vector<RegionWithPos> regions;
        for(int zorder : GetParam())
        {
            auto region = createRegion();

            ON_CALL(*region, zorder()).WillByDefault(testing::Return(zorder));
            pushRegionAndSort(region.get());

            regions.emplace_back(RegionWithPos{std::move(region), DEFAULT_XPOS, DEFAULT_YPOS});
        }
        builder.regions(std::move(regions));
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
