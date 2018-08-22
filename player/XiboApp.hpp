#pragma once

#include <gtkmm/application.h>
#include <spdlog/spdlog.h>

#include "control/CollectionInterval.hpp"
#include "parsers/CommandLineParser.hpp"
#include "xmds/XMDSManager.hpp"

class XiboApp : public Gtk::Application
{
public:
    XiboApp(const XiboApp& other) = delete;
    XiboApp& operator=(const XiboApp& other) = delete;
    ~XiboApp();

    static XiboApp& create(const std::string& name);
    static XiboApp& app();
    const CommandLineParser& command_line_parser() const;
    XMDSManager& xmds_manager();

    int run(int argc, char** argv);

private:
    XiboApp();
    int run_player();

private:
    Glib::RefPtr<Gtk::Application> m_parent_app;
    std::shared_ptr<spdlog::logger> m_logger;
    std::unique_ptr<XMDSManager> m_xmds_manager;
    CollectionInterval m_collection_inverval;
    CommandLineParser m_options;

    static std::unique_ptr<XiboApp> m_app;
};
