#pragma once

#include <memory>
#include <spdlog/common.h>

class MainLoop;
class XmdsRequestSender;
class IMainLayout;
class MainWindow;
class HttpManager;
class LayoutScheduler;
class FileCacheManager;
class CollectionInterval;
class PlayerSettingsManager;
class PlayerError;
class ScreenShoter;
class ConfigurationView;
class MainWindowController;
class Event;
struct PlayerSettings;

class XiboApp
{
public:
    XiboApp(const XiboApp& other) = delete;
    XiboApp& operator=(const XiboApp& other) = delete;
    ~XiboApp();

    static XiboApp& create(const std::string& name);
    static XiboApp& app();
    HttpManager& httpManager();
    FileCacheManager& fileManager();
    ScreenShoter& screenShoter();

    int run();

private:
    static std::vector<spdlog::sink_ptr> createLoggerSinks();

    XiboApp(const std::string& name);
    void onCollectionFinished(const PlayerError& error);
    void updateSettings(const PlayerSettings& settings);
    void updatePlayerSettings(const PlayerSettings& settings);
    bool processCallbackQueue();
    void handleCollectionUpdates(CollectionInterval& interval);
    void startPlayer(const std::shared_ptr<ConfigurationView>& view);

private:
    std::unique_ptr<MainLoop> m_mainLoop;
    std::unique_ptr<LayoutScheduler> m_scheduler;
    std::unique_ptr<FileCacheManager> m_fileManager;
    std::unique_ptr<CollectionInterval> m_collectionInterval;
    std::unique_ptr<XmdsRequestSender> m_xmdsManager;
    std::unique_ptr<HttpManager> m_httpManager;
    std::unique_ptr<PlayerSettingsManager> m_settingsManager;
    std::unique_ptr<ScreenShoter> m_screenShoter;
    std::unique_ptr<MainWindowController> m_mainWindowController;
    std::shared_ptr<MainWindow> m_mainWindow;

    static std::unique_ptr<XiboApp> m_app;
};
