#pragma once

#include "test_utils.hpp"

#include "control/MainLayout.hpp"

#include "mocks/MockOverlayAdaptor.hpp"
#include "mocks/MockBackground.hpp"
#include "mocks/MockImageAdaptor.hpp"
#include "mocks/MockMediaContainer.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

inline testing::NiceMock<MockBackground>* createBackground()
{
    auto handler = new testing::NiceMock<MockImageAdaptor>;
    auto background = new testing::NiceMock<MockBackground>(unique(handler));

    ON_CALL(*background, handler()).WillByDefault(testing::ReturnRef(*handler));

    return background;
}

inline testing::NiceMock<MockMediaContainer>* createMediaContainer()
{
    auto handler = new testing::NiceMock<MockFixedLayoutAdaptor>;
    auto mediaContainer = new testing::NiceMock<MockMediaContainer>(unique(handler));

    ON_CALL(*mediaContainer, handler()).WillByDefault(testing::ReturnRef(*handler));

    return mediaContainer;
}

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

class MainLayoutTest : public testing::Test
{
public:
    auto constructLayout()
    {
        auto layout = construct<MainLayout>(DEFAULT_WIDTH, DEFAULT_HEIGHT, unique(m_adaptor));
        addBackgroundAndContainer(layout);
        return layout;
    }

protected:
    void SetUp() override
    {
        m_adaptor = new testing::NiceMock<MockOverlayAdaptor>;
    }

    void TearDown() override
    {
        m_background = nullptr;
        m_container = nullptr;
        m_adaptor = nullptr;
    }

    MockOverlayAdaptor& adaptor()
    {
        return *m_adaptor;
    }

    MockBackground& background()
    {
        return *m_background;
    }

    MockMediaContainer& container()
    {
        return *m_container;
    }

    virtual void addContainers(std::shared_ptr<MainLayout> layout)
    {
        m_container = createMediaContainer();
        layout->addMediaContainer(unique(m_container), DEFAULT_XPOS, DEFAULT_XPOS);
    }

private:
    void addBackgroundAndContainer(std::shared_ptr<MainLayout> layout)
    {
        m_background = createBackground();
        layout->setBackground(unique(m_background));

        addContainers(layout);
    }

private:
    MockOverlayAdaptor* m_adaptor = nullptr;
    MockBackground* m_background = nullptr;
    MockMediaContainer* m_container = nullptr;

};

class MainLayoutTestPos : public MainLayoutTest, public testing::WithParamInterface<Point> { };

class MainLayoutReorderTest : public MainLayoutTest, public testing::WithParamInterface<std::vector<int>>
{
protected:
    void addContainers(std::shared_ptr<MainLayout> layout) override
    {
        for(int zorder : GetParam())
        {
            auto container = createMediaContainer();

            ON_CALL(*container, zorder()).WillByDefault(testing::Return(zorder));
            layout->addMediaContainer(unique(container), DEFAULT_XPOS, DEFAULT_YPOS);

            pushContainerAndSort(container);
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
