#pragma once

#include "BaseTestWithHandler.hpp"

#include "control/MainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"

#include "mocks/MockBackground.hpp"
#include "mocks/MockImageAdaptor.hpp"

#include "mocks/MockMediaContainer.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

const auto invalidMainLayoutPos = invalidPositions<DEFAULT_WIDTH, MIN_X_POS, DEFAULT_HEIGHT, MIN_Y_POS>;
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

        addBackgroundAndContainer(*layout);

        return layout;
    }

protected:
    MockBackground& background()
    {
        return *m_background;
    }

    MockMediaContainer& container()
    {
        return *m_container;
    }

    auto createBackground()
    {
        auto background = constructMock<MockBackground, MockImageAdaptor>();

        ON_CALL(*background, width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(*background, height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));

        return background;
    }

    auto createMediaContainer()
    {
        auto container = constructMock<MockMediaContainer, MockFixedLayoutAdaptor>();

        ON_CALL(*container, width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(*container, height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));

        return container;
    }

    virtual void addBackground(MainLayout& layout)
    {
        auto background = createBackground();
        m_background = background.get();

        layout.setBackground(std::move(background));
    }

    virtual void addContainers(MainLayout& layout)
    {
        auto container = createMediaContainer();
        m_container = container.get();

        layout.addMediaContainer(std::move(container), DEFAULT_XPOS, DEFAULT_XPOS);
    }

private:
    void addBackgroundAndContainer(MainLayout& layout)
    {
        addBackground(layout);
        addContainers(layout);
    }

private:
    MockBackground* m_background = nullptr;
    MockMediaContainer* m_container = nullptr;

};

class MainLayoutTestPos : public MainLayoutTest, public testing::WithParamInterface<Point> { };

class MainLayoutReorderTest : public MainLayoutTest, public testing::WithParamInterface<std::vector<int>>
{
protected:
    void addContainers(MainLayout& layout) override
    {
        for(int zorder : GetParam())
        {
            auto container = createMediaContainer();

            ON_CALL(*container, zorder()).WillByDefault(testing::Return(zorder));
            pushContainerAndSort(container.get());

            layout.addMediaContainer(std::move(container), DEFAULT_XPOS, DEFAULT_YPOS);
        }
    }

    testing::NiceMock<MockMediaContainer>* container(size_t index)
    {
        return m_containers[index];
    }

private:
    void pushContainerAndSort(testing::NiceMock<MockMediaContainer>* container)
    {
        m_containers.push_back(container);
        std::sort(m_containers.begin(), m_containers.end(), [=](const auto& first, const auto& second){
            return first->zorder() < second->zorder();
        });
    }

private:
    std::vector<testing::NiceMock<MockMediaContainer>*> m_containers;

};
