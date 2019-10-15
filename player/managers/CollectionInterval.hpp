#pragma once

#include "managers/CmsStatus.hpp"
#include "managers/RequiredFilesDownloader.hpp"

#include "networking/ResponseResult.hpp"
#include "networking/xmds/RegisterDisplay.hpp"
#include "networking/xmds/RequiredFiles.hpp"
#include "networking/xmds/Schedule.hpp"
#include "networking/xmds/SubmitLog.hpp"
#include "networking/xmds/SubmitStats.hpp"

#include "common/Dispatcher.hpp"
#include "common/JoinableThread.hpp"
#include "common/dt/Timer.hpp"

using CollectionResultCallback = std::function<void(const PlayerError&)>;
using SignalSettingsUpdated = Dispatcher<PlayerSettings>;
using SignalScheduleAvailable = Dispatcher<Schedule::Result>;
using SignalCollectionFinished = Dispatcher<PlayerError>;
using SignalFilesDownloaded = Dispatcher<>;
class XmdsRequestSender;

struct CollectionSession
{
    CollectionResultCallback callback;
};

using CollectionSessionPtr = std::shared_ptr<CollectionSession>;

class CollectionInterval
{
public:
    CollectionInterval(XmdsRequestSender& xmdsSender);

    void startRegularCollection();
    void stop();
    void collect(CollectionResultCallback callback);
    void updateInterval(int collectInterval);
    void statsAggregation(const std::string& aggregationLevel);
    CmsStatus status();

    SignalSettingsUpdated& settingsUpdated();
    SignalScheduleAvailable& scheduleAvailable();
    SignalCollectionFinished& collectionFinished();
    SignalFilesDownloaded& filesDownloaded();

private:
    void startTimer();

    void sessionFinished(CollectionSessionPtr session, PlayerError = {});
    void onRegularCollectionFinished(const PlayerError& error);

    void onDisplayRegistered(const ResponseResult<RegisterDisplay::Result>& registerDisplay,
                             CollectionSessionPtr session);
    PlayerError getDisplayStatus(const RegisterDisplay::Result::Status& status);
    void onRequiredFiles(const ResponseResult<RequiredFiles::Result>& requiredFiles, CollectionSessionPtr session);
    void onSchedule(const ResponseResult<Schedule::Result>& schedule, CollectionSessionPtr session);
    void updateMediaInventory(MediaInventoryItems&& items);
    void onSubmitLog(const ResponseResult<SubmitLog::Result>& logResult, CollectionSessionPtr session);
    void onSubmitStats(const ResponseResult<SubmitStats::Result>& statsResult, CollectionSessionPtr session);

private:
    XmdsRequestSender& xmdsSender_;
    std::unique_ptr<JoinableThread> workerThread_;
    std::unique_ptr<Timer> intervalTimer_;
    int collectInterval_;
    bool started_;
    bool registered_;
    DateTime lastChecked_;
    size_t requiredFiles_;
    std::string statsAggregation_;
    SignalSettingsUpdated settingsUpdated_;
    SignalScheduleAvailable scheduleAvailable_;
    SignalCollectionFinished collectionFinished_;
    SignalFilesDownloaded filesDownloaded_;
};
