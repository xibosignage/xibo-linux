#pragma once

#include <gtkmm/application.h>

class XMDSManager;
class IMainLayout;
class MainWindow;
class HTTPManager;
class Scheduler;
class FileCacheManager;
class CollectionInterval;
class CommandLineParser;
class CollectionResult;
struct PlayerSettings;

class XiboApp
{
public:
    XiboApp(const XiboApp& other) = delete;
    XiboApp& operator=(const XiboApp& other) = delete;
    ~XiboApp();

    static XiboApp& create(const std::string& name);
    static XiboApp& app();
    XMDSManager& xmdsManager();
    HTTPManager& downloadManager();
    FileCacheManager& fileManager();

    int run(int argc, char** argv);

private:
    XiboApp(const std::string& name);

    int initMainLoop();
    void runPlayer(MainWindow& window);
    void onCollectionFinished(const CollectionResult& result);
    void updateSettings(const PlayerSettings& settings);
    void tryParseCommandLine(int argc, char** argv);
    bool processCallbackQueue();

private:
    Glib::RefPtr<Gtk::Application> m_parentApp;
    std::unique_ptr<MainWindow> m_mainWindow;
    std::unique_ptr<XMDSManager> m_xmdsManager;
    std::unique_ptr<HTTPManager> m_downloadManager;
    std::unique_ptr<Scheduler> m_scheduler;
    std::unique_ptr<FileCacheManager> m_fileManager;
    std::unique_ptr<CollectionInterval> m_collectionInterval;
    std::unique_ptr<CommandLineParser> m_options;
    sigc::connection m_idleConnection;

    static std::unique_ptr<XiboApp> m_app;
};
