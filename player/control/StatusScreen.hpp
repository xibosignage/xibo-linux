#pragma once

#include "control/common/Widget.hpp"
#include "IStatusScreen.hpp"

#include <gtkmm/messagedialog.h>

class StatusScreen : public Widget<IStatusScreen>
{
public:
    StatusScreen(int width, int height);
    void setText(const std::string& text) override;

    Gtk::Widget& get() override;

private:
	Gtk::MessageDialog m_dialog;

};
