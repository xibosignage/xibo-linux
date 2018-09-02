#pragma once

#include <gtkmm/application.h>
#include <spdlog/spdlog.h>

#include "factories/CommandLineParser.hpp"
#include "control/DownloadManager.hpp"
#include "control/CollectionInterval.hpp"
#include "control/PlayerSettings.hpp"

class XMDSManager;
class IMainLayout;
class MainWindow;

class XiboApp : public Gtk::Application
{
public:
    XiboApp(const XiboApp& other) = delete;
    XiboApp& operator=(const XiboApp& other) = delete;
    ~XiboApp() override;

    static XiboApp& create(const std::string& name);
    static XiboApp& app();
    XMDSManager& xmdsManager();
    DownloadManager& downloadManager();

    int run(int argc, char** argv);

private:
    XiboApp(const std::string& name);
    int initPlayer();
    void runPlayer(MainWindow& window);
    void updateSettings(const PlayerSettings& settings);
    std::string findXlfFile();

private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::unique_ptr<IMainLayout> m_layout;
    std::unique_ptr<XMDSManager> m_xmdsManager;
    DownloadManager m_downloadManager;
    CollectionInterval m_collectionInterval;
    CommandLineParser m_options;

    static std::unique_ptr<XiboApp> m_app;
};
