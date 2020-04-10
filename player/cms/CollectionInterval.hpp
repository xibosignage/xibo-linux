#pragma once

#include "CmsStatus.hpp"
#include "RequiredFilesDownloader.hpp"

#include "cms/xmds/RegisterDisplay.hpp"
#include "cms/xmds/RequiredFiles.hpp"
#include "cms/xmds/Schedule.hpp"
#include "cms/xmds/SubmitLog.hpp"
#include "cms/xmds/SubmitStats.hpp"
#include "networking/ResponseResult.hpp"
#include "schedule/LayoutSchedule.hpp"

#include "common/JoinableThread.hpp"
#include "common/dt/Timer.hpp"

#include "XmdsCommand.hpp"

using CollectionResultCallback = std::function<void(const PlayerError&)>;
using SignalSettingsUpdated = boost::signals2::signal<void(const PlayerSettings&)>;
using SignalScheduleAvailable = boost::signals2::signal<void(LayoutSchedule)>;
using SignalCollectionFinished = boost::signals2::signal<void(const PlayerError&)>;
using SignalFilesDownloaded = boost::signals2::signal<void()>;

class XmdsRequestSender;
class StatsRecorder;
class FileCache;

class CollectionInterval
{
    static constexpr const uint DefaultInterval = 900;

public:
    CollectionInterval(XmdsRequestSender& xmdsSender, StatsRecorder& statsRecorder, FileCache& fileCache);

    bool running() const;
    void stop();
    void collectNow();
    void updateInterval(int collectInterval);
    CmsStatus status() const;

    SignalSettingsUpdated& settingsUpdated();
    SignalScheduleAvailable& scheduleAvailable();
    SignalCollectionFinished& collectionFinished();
    SignalFilesDownloaded& filesDownloaded();

private:
    void startTimer();
    void sessionFinished(const PlayerError& = {});

    template <typename Command, typename... Args>
    void runCommand(Args&&... args)
    {
        auto command = Command::create(std::forward<Args>(args)...);

        command->error().connect([this](const auto& error) { sessionFinished(error); });
        setupCommandConnections(*command);

        command->execute();
        commands_.push_back(std::move(command));
    }

    template <typename Command>
    void setupCommandConnections(Command&);
    void onDisplayRegistered();

private:
    XmdsRequestSender& xmdsSender_;
    StatsRecorder& statsRecorder_;
    FileCache& fileCache_;
    std::unique_ptr<JoinableThread> workerThread_;
    std::unique_ptr<Timer> intervalTimer_;
    std::atomic_int collectInterval_;
    std::atomic_bool running_;
    CmsStatus status_;
    SignalSettingsUpdated settingsUpdated_;
    SignalScheduleAvailable scheduleAvailable_;
    SignalCollectionFinished collectionFinished_;
    SignalFilesDownloaded filesDownloaded_;
    std::vector<std::unique_ptr<IXmdsCommand>> commands_;
};
