#pragma once

#include <gtkmm/application.h>

class XMDSManager;
class IMainLayout;
class MainWindow;
class HTTPDownloader;
class Scheduler;
class CollectionInterval;
class CommandLineParser;
struct PlayerSettings;

class XiboApp : public Gtk::Application
{
public:
    XiboApp(const XiboApp& other) = delete;
    XiboApp& operator=(const XiboApp& other) = delete;
    ~XiboApp() override;

    static XiboApp& create(const std::string& name);
    static XiboApp& app();
    XMDSManager& xmdsManager();
    HTTPDownloader& downloadManager();

    int run(int argc, char** argv);

private:
    XiboApp(const std::string& name);
    int initMainLoop();
    void runPlayer(MainWindow& window);
    void updateSettings(const PlayerSettings& settings);
    void tryParseCommandLine(int argc, char** argv);

private:
    std::unique_ptr<XMDSManager> m_xmdsManager;
    std::unique_ptr<HTTPDownloader> m_downloadManager;
    std::unique_ptr<Scheduler> m_scheduler;
    std::unique_ptr<CollectionInterval> m_collectionInterval;
    std::unique_ptr<CommandLineParser> m_options;

    static std::unique_ptr<XiboApp> m_app;
};
