#pragma once

#include "IWidgetAdaptor.hpp"
#include <gtkmm/widget.h>

class IGtkAdaptor
{
public:
    virtual ~IGtkAdaptor() = default;
    virtual Gtk::Widget& get() = 0;
};

template<typename Interface>
class GtkAdaptor : public Interface, public IGtkAdaptor
{
    static_assert(std::is_base_of_v<IWidgetAdaptor, Interface>, "Interface should implement IWidgetAdaptor");

public:
    GtkAdaptor(Gtk::Widget& handler) : m_handler(handler)
    {
    }

    void show() override;
    void hide() override;
    bool isShown() const override;

    void scale(double, double) override;
    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;

protected:
    Gtk::Widget& getHandler(IWidgetAdaptor& child)
    {
        return dynamic_cast<IGtkAdaptor&>(child).get();
    }

private:
    Gtk::Widget& m_handler;

};

template<typename Interface>
void GtkAdaptor<Interface>::show()
{
    m_handler.show();
}

template<typename Interface>
void GtkAdaptor<Interface>::hide()
{
    m_handler.hide();
}

template<typename Interface>
bool GtkAdaptor<Interface>::isShown() const
{
    return m_handler.is_visible();
}

template<typename Interface>
void GtkAdaptor<Interface>::scale(double scaleX, double scaleY)
{
    setSize(width() * scaleX, height() * scaleY);
}

template<typename Interface>
void GtkAdaptor<Interface>::setSize(int width, int height)
{
    m_handler.set_size_request(width, height);
}

template<typename Interface>
int GtkAdaptor<Interface>::width() const
{
    int width, _;
    m_handler.get_size_request(width, _);
    return width;
}

template<typename Interface>
int GtkAdaptor<Interface>::height() const
{
    int _, height;
    m_handler.get_size_request(_, height);
    return height;
}
