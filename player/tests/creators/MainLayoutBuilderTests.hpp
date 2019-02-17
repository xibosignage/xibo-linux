#pragma once

#include "test_utils.hpp"

#include "creators/MainLayoutBuilder.hpp"
#include "control/IMainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"

#include "mocks/MockBackground.hpp"
#include "mocks/MockImageAdaptor.hpp"

#include "mocks/MockRegion.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

//const int MIN_WIDTH = 1;
//const int MIN_HEIGHT = 1;

//const auto invalidMainLayoutSizes = invalidSizes<MIN_WIDTH, MIN_HEIGHT>;

//class MainLayoutBuilderSizeTest : public testing::TestWithParam<Size> { };

class MainLayoutBuilderTest : public MainLayoutBuilder
{
public:
    MainLayoutBuilderTest() = default;

    MainLayoutBuilderTest& adaptor(std::unique_ptr<testing::NiceMock<MockOverlayAdaptor>>&& adaptor)
    {
        m_adaptor = std::move(adaptor);
        return *this;
    }

protected:
    std::unique_ptr<IOverlayAdaptor> createAdaptor() override
    {
        return std::move(m_adaptor);
    }

private:
    std::unique_ptr<testing::NiceMock<MockOverlayAdaptor>> m_adaptor;

};
