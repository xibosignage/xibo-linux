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

class MainLayoutTest : public BaseTestWithHandler<MockOverlayAdaptor>
{
public:
    auto constructLayout()
    {
        auto layout = construct<MainLayout>(DEFAULT_WIDTH, DEFAULT_HEIGHT, unique(&adaptor()));

        ON_CALL(adaptor(), width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(adaptor(), height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));

        addBackgroundAndRegions(*layout);

        return layout;
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

    virtual void addBackground(MainLayout& layout)
    {
        auto background = createBackground();
        m_background = background.get();

        layout.setBackground(std::move(background));
    }

    virtual void addRegions(MainLayout& layout)
    {
        auto region = createRegion();
        m_region = region.get();

        layout.addRegion(std::move(region), DEFAULT_XPOS, DEFAULT_XPOS);
    }

private:
    void addBackgroundAndRegions(MainLayout& layout)
    {
        addBackground(layout);
        addRegions(layout);
    }

private:
    MockBackground* m_background = nullptr;
    MockRegion* m_region = nullptr;

};

class MainLayoutReorderTest : public MainLayoutTest, public testing::WithParamInterface<std::vector<int>>
{
protected:
    void addRegions(MainLayout& layout) override
    {
        for(int zorder : GetParam())
        {
            auto region = createRegion();

            ON_CALL(*region, zorder()).WillByDefault(testing::Return(zorder));
            pushRegionAndSort(region.get());

            layout.addRegion(std::move(region), DEFAULT_XPOS, DEFAULT_YPOS);
        }
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
