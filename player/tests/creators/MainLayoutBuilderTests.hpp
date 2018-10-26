#pragma once

#include "test_utils.hpp"

#include "creators/MainLayoutBuilder.hpp"
#include "mocks/MockOverlayAdaptor.hpp"

#include "mocks/MockBackground.hpp"
#include "mocks/MockImageAdaptor.hpp"

#include "mocks/MockMediaContainer.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

const auto invalidMediaContainerSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_WIDTH, MAX_DISPLAY_HEIGHT, MIN_HEIGHT>;

class MainLayoutBuilderSizeTest : public testing::TestWithParam<Size> { };

class MainLayoutBuilderTest : public MainLayoutBuilder
{
public:
    MainLayoutBuilderTest()
    {
        defaultBackground().defaultContainers();
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

    MainLayoutBuilderTest& defaultContainers()
    {
        std::vector<MediaContainerWithPos> containers;
        containers.push_back(MediaContainerWithPos{constructMock<MockMediaContainer, MockFixedLayoutAdaptor>(), DEFAULT_XPOS, DEFAULT_YPOS});
        return static_cast<MainLayoutBuilderTest&>(mediaContainers(std::move(containers)));
    }

private:
    std::unique_ptr<testing::NiceMock<MockOverlayAdaptor>> m_adaptor;

};
