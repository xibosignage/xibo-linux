#include "CollectionInterval.hpp"

#include "MainLoop.hpp"

#include "common/dt/DateTime.hpp"
#include "common/dt/Timer.hpp"
#include "common/logger/Logging.hpp"
#include "config/AppConfig.hpp"

#include "cms/xmds/XmdsRequestSender.hpp"
#include "stat/StatsRecorder.hpp"

#include "GetScheduleCommand.hpp"
#include "MediaInventoryCommand.hpp"
#include "RegisterDisplayCommand.hpp"
#include "RequiredFilesCommand.hpp"
#include "SubmitLogsCommand.hpp"
#include "SubmitStatsCommand.hpp"

namespace ph = std::placeholders;

CollectionInterval::CollectionInterval(XmdsRequestSender& xmdsSender,
                                       StatsRecorder& statsRecorder,
                                       FileCache& fileCache) :
    xmdsSender_{xmdsSender},
    statsRecorder_{statsRecorder},
    fileCache_{fileCache},
    intervalTimer_{std::make_unique<Timer>()},
    collectInterval_{DefaultInterval},
    running_{false},
    status_{}
{
    assert(intervalTimer_);
}

bool CollectionInterval::running() const
{
    return running_;
}

void CollectionInterval::stop()
{
    workerThread_.reset();
}

void CollectionInterval::startTimer()
{
    intervalTimer_->startOnce(std::chrono::seconds(collectInterval_), [this]() { collectNow(); });
}

void CollectionInterval::updateInterval(int collectInterval)
{
    if (collectInterval_ != collectInterval)
    {
        Log::debug("[CollectionInterval] Interval updated to {} seconds", collectInterval);
        collectInterval_ = collectInterval;
    }
}

// TODO potential data race here
CmsStatus CollectionInterval::status() const
{
    return status_;
}

SignalSettingsUpdated& CollectionInterval::settingsUpdated()
{
    return settingsUpdated_;
}

SignalScheduleAvailable& CollectionInterval::scheduleAvailable()
{
    return scheduleAvailable_;
}

SignalCollectionFinished& CollectionInterval::collectionFinished()
{
    return collectionFinished_;
}

SignalFilesDownloaded& CollectionInterval::filesDownloaded()
{
    return filesDownloaded_;
}

void CollectionInterval::collectNow()
{
    if (!running_)
    {
        running_ = true;
        workerThread_ = std::make_unique<JoinableThread>([=]() {
            Log::debug("[CollectionInterval] Started");

            runCommand<RegisterDisplayCommand>(xmdsSender_.getHost(),
                                               xmdsSender_.getServerKey(),
                                               xmdsSender_.getHardwareKey(),
                                               AppConfig::version(),
                                               AppConfig::codeVersion(),
                                               "Display");
        });
    }
}

void CollectionInterval::sessionFinished(const PlayerError& error)
{
    running_ = false;
    commands_.clear();
    startTimer();
    Log::debug("[CollectionInterval] Finished. Next collection will start in {} seconds", collectInterval_);

    MainLoop::pushToUiThread([this, error]() { collectionFinished_(error); });
}

void CollectionInterval::onDisplayRegistered()
{
    Log::debug("[XMDS::RegisterDisplay] Success");

    runCommand<RequiredFilesCommand>(xmdsSender_.getHost(), xmdsSender_.getServerKey(), xmdsSender_.getHardwareKey());
    runCommand<GetScheduleCommand>(xmdsSender_.getHost(), xmdsSender_.getServerKey(), xmdsSender_.getHardwareKey());
    runCommand<SubmitLogsCommand>(xmdsSender_.getHost(), xmdsSender_.getServerKey(), xmdsSender_.getHardwareKey());

    if (!statsRecorder_.empty())
    {
        runCommand<SubmitStatsCommand>(
            xmdsSender_.getHost(), xmdsSender_.getServerKey(), xmdsSender_.getHardwareKey(), statsRecorder_);
        statsRecorder_.clear();
    }
}

template <>
void CollectionInterval::setupCommandConnections(RegisterDisplayCommand& command)
{
    command.displayReady().connect([this]() {
        status_.registered = true;
        status_.lastChecked = DateTime::now();
        onDisplayRegistered();
    });

    command.settingsUpdated().connect([this](auto settings) {
        MainLoop::pushToUiThread([this, settings = std::move(settings)]() { settingsUpdated_(settings); });
    });
}

template <>
void CollectionInterval::setupCommandConnections(RequiredFilesCommand& command)
{
    command.filesReady().connect([this](const auto& files, const auto& resources) {
        Log::debug("[XMDS::RequiredFiles] Received");

        RequiredFilesDownloader downloader{xmdsSender_, fileCache_};

        status_.requiredFiles = files.size() + resources.size();

        auto resourcesResult = downloader.download(resources);
        auto filesResult = downloader.download(files);

        runCommand<MediaInventoryCommand>(
            xmdsSender_.getHost(), xmdsSender_.getServerKey(), xmdsSender_.getHardwareKey(), filesResult.get());
        runCommand<MediaInventoryCommand>(
            xmdsSender_.getHost(), xmdsSender_.getServerKey(), xmdsSender_.getHardwareKey(), resourcesResult.get());

        MainLoop::pushToUiThread([this]() { filesDownloaded_(); });
    });
}

template <>
void CollectionInterval::setupCommandConnections(GetScheduleCommand& command)
{
    command.scheduleReady().connect([this](auto schedule) {
        MainLoop::pushToUiThread([this, schedule = std::move(schedule)]() { scheduleAvailable_(schedule); });
    });
}

template <>
void CollectionInterval::setupCommandConnections(SubmitLogsCommand& command)
{
    command.success().connect([]() { Log::debug("[SubmitLog] Success"); });
    command.failed().connect([]() { Log::error("[SubmitLog] Failed"); });
}

template <>
void CollectionInterval::setupCommandConnections(SubmitStatsCommand& command)
{
    command.success().connect([]() { Log::debug("[SubmitStats] Success"); });
    command.failed().connect([]() { Log::error("[SubmitStats] Failed"); });
}

template <>
void CollectionInterval::setupCommandConnections(MediaInventoryCommand& command)
{
    command.success().connect([]() { Log::debug("[MediaInventory] Success"); });
    command.failed().connect([]() { Log::error("[MediaInventory] Failed"); });
}
