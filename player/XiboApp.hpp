#pragma once

#include <memory>
#include <spdlog/common.h>

class MainLoop;
class XmdsRequestSender;
class IMainLayout;
class MainWindow;
class HttpManager;
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
    HttpManager& httpManager();
    FileCacheManager& fileManager();

    int run(int argc, char** argv);

private:
    static std::vector<spdlog::sink_ptr> createLoggerSinks();

    XiboApp(const std::string& name);
    int runMainLoop();
    void startWindow(MainWindow& window);
    void onCollectionFinished(const CollectionResult& result);
    void updateSettings(const PlayerSettings& settings);
    void tryParseCommandLine(int argc, char** argv);
    bool processCallbackQueue();

private:
    std::unique_ptr<MainLoop> m_mainLoop;
    std::unique_ptr<Scheduler> m_scheduler;
    std::unique_ptr<FileCacheManager> m_fileManager;
    std::unique_ptr<CollectionInterval> m_collectionInterval;
    std::unique_ptr<XmdsRequestSender> m_xmdsManager;
    std::unique_ptr<HttpManager> m_httpManager;
    std::unique_ptr<CommandLineParser> m_options;

    static std::unique_ptr<XiboApp> m_app;
};
