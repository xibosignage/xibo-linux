#pragma once

#include "control/widgets/Widget.hpp"

#include <memory>

namespace Xibo
{
    class Container : public Widget
    {
    public:
        virtual void remove(const std::shared_ptr<Xibo::Widget>& child) = 0;
        virtual void removeAll() = 0;
    };
}

template <typename Base, typename WidgetInfo = void*>
class Container : public Base
{
    static_assert(std::is_base_of_v<Xibo::Container, Base>, "Should implement Xibo::Container");

    struct WidgetWithInfo
    {
        std::shared_ptr<Xibo::Widget> widget;
        WidgetInfo info;
    };

public:
    void setSize(int targetWidth, int targetHeight) override
    {
        auto scaleFactor = calcScaleFactor(this->width(), this->height(), targetWidth, targetHeight);

        Base::setSize(targetWidth, targetHeight);
        scaleChildren(scaleFactor);
    }

    void remove(const std::shared_ptr<Xibo::Widget>& widget) override
    {
        assert(widget);

        removeFromContainer(widget);
        removeFromHandler(widget);
    }

    void removeAll() override
    {
        for (auto&& [widget, info] : children_)
        {
            remove(widget);
        }
        children_.clear();
    }

protected:
    using Base::Base;
    using Widgets = std::vector<WidgetWithInfo>;

    virtual void scaleChild(const std::shared_ptr<Xibo::Widget>& widget, WidgetInfo& info, double scaleFactor) = 0;
    virtual void removeFromHandler(const std::shared_ptr<Xibo::Widget>& widget) = 0;

    void addToContainer(const std::shared_ptr<Xibo::Widget>& widget, const WidgetInfo& info)
    {
        children_.emplace_back(WidgetWithInfo{widget, info});
    }

    void removeFromContainer(const std::shared_ptr<Xibo::Widget>& widget)
    {
        auto predicate = [widget](const auto& other) { return other.widget == widget; };
        children_.erase(findChild(predicate), children_.end());
    }

    void scaleChildren(double scaleFactor)
    {
        for (auto&& [widget, info] : children_)
        {
            scaleChild(widget, info, scaleFactor);
        }
    }

    double calcScaleFactor(int originalWidth, int originalHeight, int targetWidth, int targetHeight)
    {
        auto scaleFactor = std::min(targetWidth / static_cast<double>(originalWidth),
                                    targetHeight / static_cast<double>(originalHeight));

        if (scaleFactor > 1)
        {
            scaleFactor = std::max(targetWidth / static_cast<double>(originalWidth),
                                   targetHeight / static_cast<double>(originalHeight));
        }

        return scaleFactor;
    }

    template <typename Callback>
    typename Widgets::iterator findChild(Callback clbk)
    {
        return std::find_if(children_.begin(), children_.end(), clbk);
    }

    const Widgets& children() const
    {
        return children_;
    }

    Widgets& children()
    {
        return children_;
    }

private:
    Widgets children_;
};
