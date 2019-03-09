#pragma once

#include <memory>

class MainLoop;
class XMDSManager;
class IMainLayout;
class MainWindow;
class HTTPManager;
class Scheduler;
class FileCacheManager;
class CollectionInterval;
class CommandLineParser;
class PlayerSettingsManager;
class PlayerError;
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
    HTTPManager& httpManager();
    FileCacheManager& fileManager();

    int run(int argc, char** argv);

private:
    XiboApp(const std::string& name);

    int runMainLoop();
    void startWindow(MainWindow& window);
    void onCollectionFinished(const PlayerError& error);
    void updateSettings(const PlayerSettings& settings);
    void updatePlayerSettings(const PlayerSettings& settings);
    void tryParseCommandLine(int argc, char** argv);
    bool processCallbackQueue();

private:
    std::unique_ptr<MainLoop> m_mainLoop;
    std::unique_ptr<Scheduler> m_scheduler;
    std::unique_ptr<FileCacheManager> m_fileManager;
    std::unique_ptr<CollectionInterval> m_collectionInterval;
    std::unique_ptr<XMDSManager> m_xmdsManager;
    std::unique_ptr<HTTPManager> m_httpManager;
    std::unique_ptr<CommandLineParser> m_options;
    std::unique_ptr<PlayerSettingsManager> m_settingsManager;

    static std::unique_ptr<XiboApp> m_app;
};
