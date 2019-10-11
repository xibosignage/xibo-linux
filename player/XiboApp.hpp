#pragma once

#include <memory>
#include <spdlog/common.h>

#include "common/settings/CmsSettings.hpp"
#include "common/settings/PlayerSettings.hpp"
#include "control/GeneralInfo.hpp"

#include "control/ApplicationWindow.hpp"
#include "control/widgets/gtk/WindowGtk.hpp"

class MainLoop;
class XmdsRequestSender;
class HttpClient;
class Scheduler;
class FileCache;
class CollectionInterval;
class PlayerError;
class ScreenShoter;
class XmrManager;
using ApplicationWindowGtk = ApplicationWindow<WindowGtk>;
class XiboWebServer;
class LayoutsManager;
class XiboApp;

XiboApp& xiboApp();

class XiboApp
{
public:
    XiboApp(const XiboApp& other) = delete;
    XiboApp& operator=(const XiboApp& other) = delete;
    ~XiboApp();

    static XiboApp& create(const std::string& name);

    FileCache& fileManager();
    ScreenShoter& screenShoter();
    XiboWebServer& webserver();

    int run();

private:
    static std::vector<spdlog::sink_ptr> createLoggerSinks();

    XiboApp(const std::string& name);
    void setupXmrManager();
    void setupLayoutManager();
    std::unique_ptr<CollectionInterval> createCollectionInterval(XmdsRequestSender& xmdsManager);

    void onCollectionFinished(const PlayerError& error);
    void updateAndApplySettings(const PlayerSettings& settings);
    void applyPlayerSettings(const PlayerSettings& settings);

    GeneralInfo collectGeneralInfo();

private:
    std::unique_ptr<MainLoop> mainLoop_;
    std::unique_ptr<FileCache> fileCache_;
    std::unique_ptr<Scheduler> scheduler_;
    std::unique_ptr<CollectionInterval> collectionInterval_;
    std::unique_ptr<XmdsRequestSender> xmdsManager_;
    std::unique_ptr<ScreenShoter> screenShoter_;
    std::unique_ptr<XmrManager> xmrManager_;
    std::shared_ptr<ApplicationWindowGtk> mainWindow_;
    std::shared_ptr<XiboWebServer> webserver_;
    std::unique_ptr<LayoutsManager> layoutsManager_;
    CmsSettings cmsSettings_;
    PlayerSettings playerSettings_;
};
