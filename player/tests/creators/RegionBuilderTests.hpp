#pragma once

#include "test_utils.hpp"
#include "creators/RegionBuilder.hpp"

#include "control/IRegion.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"
#include "mocks/MockRegionContent.hpp"
#include "mocks/MockWidgetAdaptor.hpp"

//const int MIN_WIDTH = 1;
//const int MIN_HEIGHT = 1;
//const auto invalidRegionSizes = invalidSizes<MIN_WIDTH, MIN_HEIGHT>;

//class RegionBuilderSizeTest : public testing::TestWithParam<Size> { };

class RegionBuilderTest : public RegionBuilder
{
public:
    RegionBuilderTest& adaptor(std::unique_ptr<testing::NiceMock<MockFixedLayoutAdaptor>>&& adaptor)
    {
        m_adaptor = std::move(adaptor);
        return *this;
    }

protected:
    std::unique_ptr<IFixedLayoutAdaptor> createAdaptor() override
    {
        return std::move(m_adaptor);
    }

private:
    std::unique_ptr<testing::NiceMock<MockFixedLayoutAdaptor>> m_adaptor;

};
