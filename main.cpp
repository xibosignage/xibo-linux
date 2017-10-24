#include "MainLayout.hpp"
#include <gtkmm/application.h>

#include <iostream>

int main (int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
  MainLayout layout;

  return app->run(layout);
}
