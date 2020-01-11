#pragma once

#include "control/widgets/FixedContainer.hpp"

#include <memory>

namespace Xibo
{
    class OverlayContainer : public FixedContainer
    {
    public:
        virtual void setMainChild(const std::shared_ptr<Xibo::Widget>& child) = 0;
        virtual void removeMainChild() = 0;
        virtual void removeOverlays() = 0;
    };
}

namespace OverlayContainerFactory
{
    std::unique_ptr<Xibo::OverlayContainer> create(int width, int height);
}

template <typename Base>
class OverlayContainer : public Base
{
    static constexpr const int DefaultPos = 0;
    static constexpr const int MainChildOrder = 0;
    static constexpr const int OverlaysOrder = 1;

    static_assert(std::is_base_of_v<Xibo::OverlayContainer, Base>, "Should implement Xibo::OverlayContainer");

    friend std::unique_ptr<Xibo::OverlayContainer> OverlayContainerFactory::create(int width, int height);

public:
    void setMainChild(const std::shared_ptr<Xibo::Widget>& mainChild)
    {
        assert(mainChild);

        removePreviousMainChild();

        Base::add(mainChild, DefaultPos, DefaultPos, MainChildOrder);
        mainChild_ = mainChild;
    }

    void removeMainChild()
    {
        Base::remove(mainChild_);
        mainChild_.reset();
    }

    void add(const std::shared_ptr<Xibo::Widget>& child, int left, int top, int zorder) override
    {
        assert(child);

        overlayContainer_->add(child, left, top, zorder);
    }

    void remove(const std::shared_ptr<Xibo::Widget>& child) override
    {
        assert(child);

        overlayContainer_->remove(child);
    }

    void removeOverlays()
    {
        overlayContainer_->removeAll();
    }

private:
    void setOverlayContainer(const std::shared_ptr<Xibo::FixedContainer>& container)
    {
        assert(container);

        Base::add(container, DefaultPos, DefaultPos, OverlaysOrder);
        overlayContainer_ = container;
    }

    void removePreviousMainChild()
    {
        if (mainChild_)
        {
            removeMainChild();
        }
    }

private:
    std::shared_ptr<Xibo::FixedContainer> overlayContainer_;
    std::shared_ptr<Xibo::Widget> mainChild_;
};
