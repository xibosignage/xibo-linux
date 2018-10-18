#pragma once

#include "test_utils.hpp"

#include "creators/MainLayoutBuilder.hpp"

#include "mocks/MockBackground.hpp"
#include "mocks/MockImageAdaptor.hpp"
#include "mocks/MockMainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"
#include "mocks/MockMediaContainer.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

class MainLayoutBuilderTest : public MainLayoutBuilder
{
public:
    MainLayoutBuilderTest& fakeLayout(std::unique_ptr<IMainLayout>&& layout)
    {
        m_layout = std::move(layout);
        return static_cast<MainLayoutBuilderTest&>(*this);
    }

    MainLayoutBuilderTest& defaultAdaptor()
    {
        return static_cast<MainLayoutBuilderTest&>(adaptor(std::make_unique<testing::NiceMock<MockOverlayAdaptor>>()));
    }

    MainLayoutBuilderTest& defaultBackground()
    {
        return static_cast<MainLayoutBuilderTest&>(background(fake_construct<MockBackground, MockImageAdaptor>()));
    }

    MainLayoutBuilderTest& defaultContainers()
    {
        std::vector<MediaContainerWithPos> containers;
        containers.push_back(MediaContainerWithPos{fake_construct<MockMediaContainer, MockFixedLayoutAdaptor>(), DEFAULT_XPOS, DEFAULT_YPOS});
        return static_cast<MainLayoutBuilderTest&>(mediaContainers(std::move(containers)));
    }

    MainLayoutBuilder& defaultSize()
    {
        return width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT);
    }

protected:
    std::unique_ptr<IMainLayout> createLayout() override
    {
        if(m_layout)
            return std::move(m_layout);

        return fake_construct<MockMainLayout, MockOverlayAdaptor>();
    }

private:
    std::unique_ptr<IMainLayout> m_layout;

};
