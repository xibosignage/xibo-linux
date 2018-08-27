#pragma once

#include <gtkmm/application.h>
#include <spdlog/spdlog.h>

#include "parsers/CommandLineParser.hpp"
#include "control/DownloadManager.hpp"
#include "control/CollectionInterval.hpp"
#include "control/PlayerSettings.hpp"

class XMDSManager;
class MainLayout;
class MainWindow;

class XiboApp : public Gtk::Application
{
public:
    XiboApp(const XiboApp& other) = delete;
    XiboApp& operator=(const XiboApp& other) = delete;
    ~XiboApp() override;

    static XiboApp& create(const std::string& name);
    static XiboApp& app();
    XMDSManager& xmds_manager();
    DownloadManager& download_manager();

    int run(int argc, char** argv);

private:
    XiboApp(const std::string& name);
    int init_player();
    void run_player(MainWindow& window);
    void update_settings(const PlayerSettings& settings);
    std::string find_xlf_file();

private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::unique_ptr<MainLayout> m_layout;
    std::unique_ptr<XMDSManager> m_xmds_manager;
    DownloadManager m_download_manager;
    CollectionInterval m_collection_interval;
    CommandLineParser m_options;

    static std::unique_ptr<XiboApp> m_app;
};
