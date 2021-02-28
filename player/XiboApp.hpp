#pragma once

#include "control/ApplicationWindow.hpp"
#include "control/status/GeneralInfo.hpp"
#include "control/widgets/gtk/WindowGtk.hpp"

#include "config/CmsSettings.hpp"
#include "config/PlayerSettings.hpp"

#include <boost/noncopyable.hpp>
#include <memory>

class MainLoop;
class XmdsRequestSender;
class HttpClient;
class Scheduler;
class FileCache;
class CollectionInterval;
class ScreenShotInterval;
class PlayerError;
class XmrManager;
using ApplicationWindowGtk = ApplicationWindow<WindowGtk>;
class LocalWebServer;
namespace Stats
{
    class Recorder;
}
class LayoutsManager;
class XiboApp;
struct PlayerSettings;

XiboApp& xiboApp();

class XiboApp : boost::noncopyable
{
public:
    static XiboApp& create(const std::string& name);
    int run();

    static Uri localAddress();

private:
    XiboApp(const std::string& name);

    std::unique_ptr<XmrManager> createXmrManager();
    std::shared_ptr<ApplicationWindowGtk> createMainWindow();
    std::unique_ptr<LayoutsManager> createLayoutManager();
    std::unique_ptr<CollectionInterval> createCollectionInterval(XmdsRequestSender& xmdsManager);
    std::unique_ptr<ScreenShotInterval> createScreenshotInterval(XmdsRequestSender& xmdsManager, Xibo::Window& window);

    void onCollectionFinished(const PlayerError& error);
    GeneralInfo collectGeneralInfo();
    void checkResourceDirectory();

private:
    PlayerSettings playerSettings_;
    CmsSettings cmsSettings_;

    std::unique_ptr<MainLoop> mainLoop_;
    std::unique_ptr<FileCache> fileCache_;
    std::unique_ptr<Scheduler> scheduler_;
    std::unique_ptr<CollectionInterval> collectionInterval_;
    std::unique_ptr<ScreenShotInterval> screenShotInterval_;
    std::unique_ptr<XmdsRequestSender> xmdsManager_;
    std::unique_ptr<Stats::Recorder> statsRecorder_;
    std::unique_ptr<XmrManager> xmrManager_;
    std::shared_ptr<ApplicationWindowGtk> mainWindow_;
    std::shared_ptr<LocalWebServer> webserver_;
    std::unique_ptr<LayoutsManager> layoutManager_;
};
