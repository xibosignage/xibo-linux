#pragma once

#include "test_utils.hpp"

#include "creators/MainLayoutBuilder.hpp"
#include "control/IMainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"

#include "mocks/MockBackground.hpp"
#include "mocks/MockImageAdaptor.hpp"

#include "mocks/MockRegion.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

const auto invalidMainLayoutSizes = invalidSizes<MIN_WIDTH, MIN_HEIGHT>;

class MainLayoutBuilderSizeTest : public testing::TestWithParam<Size> { };

class MainLayoutBuilderTest : public MainLayoutBuilder
{
public:
    MainLayoutBuilderTest()
    {
        defaultBackground();
    }

    MainLayoutBuilderTest& adaptor(std::unique_ptr<testing::NiceMock<MockOverlayAdaptor>>&& adaptor)
    {
        m_adaptor = std::move(adaptor);
        return *this;
    }

    MainLayoutBuilder& defaultSize()
    {
        return width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT);
    }

    MainLayoutBuilderTest& defaultRegions()
    {
        std::vector<RegionWithPos> allRegions;
        allRegions.push_back(RegionWithPos{constructMock<MockRegion, MockFixedLayoutAdaptor>(), DEFAULT_XPOS, DEFAULT_YPOS});
        return static_cast<MainLayoutBuilderTest&>(regions(std::move(allRegions)));
    }

protected:
    std::unique_ptr<IOverlayAdaptor> createAdaptor() override
    {
        if(m_adaptor)
            return std::move(m_adaptor);

        return std::make_unique<testing::NiceMock<MockOverlayAdaptor>>();
    }

private:
    MainLayoutBuilderTest& defaultBackground()
    {
        return static_cast<MainLayoutBuilderTest&>(background(constructMock<MockBackground, MockImageAdaptor>()));
    }


private:
    std::unique_ptr<testing::NiceMock<MockOverlayAdaptor>> m_adaptor;

};
