#pragma once

#include <gtkmm/application.h>

#include <spdlog/spdlog.h>
#include <boost/program_options.hpp>

class XiboApp : public Gtk::Application
{
public:
    XiboApp(const std::string& app_name);
    int run(int argc, char** argv);

    static const std::string& example_dir();

private:
    void init();
    int parse_command_line(int argc, char** argv);
    std::string get_xlf_file();

private:
    enum ParsedResult
    {
        Success = 0,
        Failure = -1
    };

private:
    Glib::RefPtr<Gtk::Application> m_app;
    boost::program_options::variables_map m_vm;
    std::shared_ptr<spdlog::logger> m_logger;

    static std::string s_example_dir;

};
