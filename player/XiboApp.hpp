#pragma once

#include <gtkmm/application.h>
#include <spdlog/spdlog.h>

#include "parsers/CommandLineParser.hpp"
#include "control/DownloadManager.hpp"
#include "control/CollectionInterval.hpp"

class XMDSManager;

class XiboApp : public Gtk::Application
{
public:
    XiboApp(const XiboApp& other) = delete;
    XiboApp& operator=(const XiboApp& other) = delete;
    ~XiboApp() override;

    static XiboApp& create(const std::string& name);
    static XiboApp& app();
    const CommandLineParser& command_line_parser() const;
    XMDSManager& xmds_manager();
    DownloadManager& download_manager();

    int run(int argc, char** argv);

private:
    XiboApp(const std::string& name);
    int run_player();

private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::unique_ptr<XMDSManager> m_xmds_manager;
    DownloadManager m_download_manager;
    CollectionInterval m_collection_interval;
    CommandLineParser m_options;

    static std::unique_ptr<XiboApp> m_app;
};
