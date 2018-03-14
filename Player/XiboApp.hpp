#pragma once

#include <gtkmm/application.h>
#include <spdlog/spdlog.h>

class XiboApp : public Gtk::Application
{
public:
    XiboApp(const std::string& app_name);
    int run(int argc, char** argv);

    static const std::string& example_dir();

private:
    void init();
    void parse_command_line(int argc, char** argv);
    std::string get_xlf_file();

private:
    Glib::RefPtr<Gtk::Application> m_app;
    std::shared_ptr<spdlog::logger> m_logger;

    static std::string s_example_dir;

};
