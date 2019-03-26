#pragma once

#include "constants.hpp"
#include "view/Widget.hpp"
#include "events/EventPublisher.hpp"

#include <gtkmm/button.h>
#include <gtkmm/messagedialog.h>

class StatusScreen : public Widget
{
public:
    StatusScreen(int width, int height);
	void setText(const std::string& text);
    void showDialog();

    Gtk::Widget& get() override;

private:
	Gtk::Button m_configButton;
	Gtk::MessageDialog m_dialog;

};
