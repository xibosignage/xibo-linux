#pragma once

#include "view/Widget.hpp"

#include <gtkmm/messagedialog.h>

class StatusScreen : public Widget
{
public:
    StatusScreen(int width, int height);
	void setText(const std::string& text);

    Gtk::Widget& get() override;

private:
	Gtk::MessageDialog m_dialog;

};
