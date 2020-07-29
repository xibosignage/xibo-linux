#pragma once

#include "control/widgets/Container.hpp"

#include <memory>

namespace Xibo
{
    class SingleContainer : public Container
    {
    public:
        virtual void add(const std::shared_ptr<Xibo::Widget>& child) = 0;
    };
}

template <typename Base>
class SingleContainer : public Container<Base>
{
    static_assert(std::is_base_of_v<Xibo::SingleContainer, Base>, "Should implement Xibo::SingleContainer");

public:
    using Container<Base>::Container;

    void add(const std::shared_ptr<Xibo::Widget>& widget) override
    {
        assert(widget);

        this->addToContainer(widget, nullptr);
        addToHandler(widget);
    }

protected:
    virtual void addToHandler(const std::shared_ptr<Xibo::Widget>& widget) = 0;

private:
    void scaleChild(const std::shared_ptr<Xibo::Widget>& widget, void*&, double scaleFactor) override
    {
        assert(widget);

        widget->scale(scaleFactor, scaleFactor);
    }
};
