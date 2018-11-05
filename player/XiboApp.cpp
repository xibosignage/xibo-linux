#include "XiboApp.hpp"
#include "config.hpp"
#include "utils/Utilities.hpp"
#include "utils/Resources.hpp"

#include "xmds/XMDSManager.hpp"
#include "control/MainLayout.hpp"
#include "control/MainWindow.hpp"
#include "creators/MainDirector.hpp"
#include "adaptors/GtkWindowAdaptor.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <glibmm/main.h>
#include <gst/gst.h>
#include <chrono>

const std::string DEFAULT_RESOURCES_DIR = "resources";
std::unique_ptr<XiboApp> XiboApp::m_app;

XiboApp& XiboApp::create(const std::string& name)
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    gst_init(nullptr, nullptr);
    Resources::setDirectory(std::filesystem::current_path() / DEFAULT_RESOURCES_DIR);

    m_app = std::unique_ptr<XiboApp>(new XiboApp(name));
    return *m_app;
}

XiboApp::XiboApp(const std::string& name) : Gtk::Application(name)
{
}

int XiboApp::initPlayer()
{
    auto window = std::make_unique<MainWindow>(std::make_unique<GtkWindowAdaptor>());
    auto&& windowHandler = static_cast<GtkWindowAdaptor&>(window->handler()).get();

    signal_startup().connect([this, &windowHandler](){
        Gtk::Application::add_window(windowHandler);
    });

    if(m_options.credentials())
    {
        m_xmdsManager.reset(new XMDSManager{m_options.host(), m_options.serverKey(), m_options.hardwareKey()});

        auto runPlayer = std::bind(&XiboApp::runPlayer, this, std::ref(*window));
        m_collectionInterval.signalFinished().connect(runPlayer);

        auto updateSettings = std::bind(&XiboApp::updateSettings, this, std::placeholders::_1);
        m_collectionInterval.signalSettingsUpdated().connect(updateSettings);

        m_collectionInterval.start();
    }
    else
    {
        runPlayer(*window);
    }

    return Gtk::Application::run();
}

void XiboApp::runPlayer(MainWindow& window)
{
    if(!window.isVisible())
    {
        Utils::logger()->info("Player started");
        MainDirector director;
        auto parsedXlfTree = Utils::parseXmlFromPath(findXlfFile());

        window.setSize(1366, 768);
        window.setFullscreen(true);
        window.addLayout(director.buildLayoutWithChildren(parsedXlfTree));
        window.showLayout();
    }
}

void XiboApp::updateSettings(const PlayerSettings& )
{
//    spdlog::set_level(static_cast<spdlog::level::level_enum>(settings.log_level));
}

// FIXME temporary workaround
std::string XiboApp::findXlfFile()
{
    namespace fs = std::filesystem;

    fs::directory_iterator it(Resources::directory());
    fs::directory_iterator end;

    while(it != end)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ".xlf")
            return it->path().string();
        ++it;
    }

    throw std::runtime_error(".XLF file was not found");
}

XiboApp::~XiboApp()
{
    if(gst_is_initialized())
    {
        gst_deinit();
    }
}

XiboApp& XiboApp::app()
{
    return *m_app;
}

XMDSManager& XiboApp::xmdsManager()
{
    return *m_xmdsManager;
}

DownloadManager& XiboApp::downloadManager()
{
    return m_downloadManager;
}

int XiboApp::run(int argc, char** argv)
{
    try
    {
        m_options.parse(argc, argv);

        if(m_options.helpOption())
        {
            Utils::logger()->info("{}", m_options.availableOptions());
        }
        else
        {
            if(m_options.versionOption())
            {
                Utils::logger()->info("Project version: {}", getVersion());
            }
            if(m_options.exampleDir())
            {
                Resources::setDirectory(m_options.getExampleDir());
            }
            if(m_options.credentials() || m_options.exampleDir())
            {
                return initPlayer();
            }
        }
    }
    catch(std::exception& ex)
    {
        Utils::logger()->error(ex.what());
        return -1;
    }
    return 0;
}
