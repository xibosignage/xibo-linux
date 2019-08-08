#pragma once

#include <memory>
#include <spdlog/common.h>

#include "common/settings/CmsSettings.hpp"
#include "common/settings/PlayerSettings.hpp"
#include "control/GeneralInfo.hpp"

class MainLoop;
class XmdsRequestSender;
class HttpClient;
class Scheduler;
class FileCache;
class CollectionInterval;
class PlayerError;
class ScreenShoter;
class XmrManager;
class MainWindowController;
class MainWindow;
class XiboWebServer;
class LayoutsManager;

class XiboApp
{
public:
    XiboApp(const XiboApp& other) = delete;
    XiboApp& operator=(const XiboApp& other) = delete;
    ~XiboApp();

    static XiboApp& create(const std::string& name);
    static XiboApp& app();

    FileCache& fileManager();
    ScreenShoter& screenShoter();
    XiboWebServer& webserver();

    int run();

private:
    static std::vector<spdlog::sink_ptr> createLoggerSinks();
    static void registerVideoSink();

    XiboApp(const std::string& name);
    void setupXmrManager();
    std::unique_ptr<CollectionInterval> createCollectionInterval(XmdsRequestSender& xmdsManager);

    void onCollectionFinished(const PlayerError& error);
    void updateAndApplySettings(const PlayerSettings& settings);
    void applyPlayerSettings(const PlayerSettings& settings);

    GeneralInfo collectGeneralInfo();

private:
    std::unique_ptr<MainLoop> m_mainLoop;
    std::unique_ptr<FileCache> m_fileCache;
    std::unique_ptr<Scheduler> m_scheduler;
    std::unique_ptr<CollectionInterval> m_collectionInterval;
    std::unique_ptr<XmdsRequestSender> m_xmdsManager;
    std::unique_ptr<ScreenShoter> m_screenShoter;
    std::unique_ptr<XmrManager> m_xmrManager;
    std::shared_ptr<MainWindow> m_mainWindow;
    std::unique_ptr<MainWindowController> m_windowController;
    std::shared_ptr<XiboWebServer> m_webserver;
    std::unique_ptr<LayoutsManager> m_layoutsManager;
    CmsSettings m_cmsSettings;
    PlayerSettings m_playerSettings;

    static std::unique_ptr<XiboApp> m_app;
};
