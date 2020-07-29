#pragma once

#include "common/PlayerRuntimeError.hpp"
#include "control/widgets/Container.hpp"

#include <memory>

namespace Xibo
{
    class FixedContainer : public Container
    {
    public:
        virtual void add(const std::shared_ptr<Xibo::Widget>& child, int left, int top, int zorder) = 0;
        virtual void reorder(const std::shared_ptr<Xibo::Widget>& child, int zorder) = 0;
    };
}

namespace FixedContainerFactory
{
    std::unique_ptr<Xibo::FixedContainer> create(int width, int height);
}

struct ChildInfo
{
    int left;
    int top;
    int zorder;
};

template <typename Base>
class FixedContainer : public Container<Base, ChildInfo>
{
    friend std::unique_ptr<Xibo::FixedContainer> FixedContainerFactory::create(int width, int height);

public:
    void add(const std::shared_ptr<Xibo::Widget>& widget, int left, int top, int zorder) override
    {
        assert(widget);

        this->addToContainer(widget, ChildInfo{left, top, zorder});
        addToHandler(widget, left, top, zorder);
        reorderChildren();
    }

    void reorder(const std::shared_ptr<Xibo::Widget>& widget, int zorder) override
    {
        assert(widget);
        assert(zorder >= 0);

        reorderInHandler(widget, zorder);
    }

protected:
    using Container<Base, ChildInfo>::Container;

    virtual void addToHandler(const std::shared_ptr<Xibo::Widget>& widget, int left, int top, int zorder) = 0;
    virtual void reorderInHandler(const std::shared_ptr<Xibo::Widget>& widget, int zorder) = 0;

    void scaleChild(const std::shared_ptr<Xibo::Widget>& widget, ChildInfo& info, double scaleFactor) override
    {
        info.left = static_cast<int>(info.left * scaleFactor);
        info.top = static_cast<int>(info.top * scaleFactor);
        widget->scale(scaleFactor, scaleFactor);
    }

    void reorderChildren()
    {
        sortChildrenByZindex();

        for (size_t i = 0; i < this->children().size(); ++i)
        {
            int zorder = static_cast<int>(i);

            assert(zorder >= 0);
            reorder(this->children()[i].widget, zorder);
        }
    }

    void sortChildrenByZindex()
    {
        std::stable_sort(this->children().begin(), this->children().end(), [=](const auto& first, const auto& second) {
            return first.info.zorder < second.info.zorder;
        });
    }
};
