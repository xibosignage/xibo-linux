#pragma once

#include <gtkmm/application.h>
#include <spdlog/spdlog.h>

#include "parsers/CommandLineParser.hpp"

class XiboApp : public Gtk::Application
{
public:
    XiboApp(const XiboApp& other) = delete;
    XiboApp& operator=(const XiboApp& other) = delete;

    static XiboApp& create(const std::string& name);
    static const XiboApp& app();

    const CommandLineParser& command_line_parser() const;

    int run(int argc, char** argv);

private:
    XiboApp();

    void init();
    void parse_command_line(int argc, char** argv);
    std::string get_xlf_file();

private:
    Glib::RefPtr<Gtk::Application> m_parent_app;
    std::shared_ptr<spdlog::logger> m_logger;
    CommandLineParser m_options;

    static std::unique_ptr<XiboApp> m_app;
};
