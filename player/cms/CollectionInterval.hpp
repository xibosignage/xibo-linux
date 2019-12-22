#pragma once

#include "CmsStatus.hpp"
#include "RequiredFilesDownloader.hpp"

#include "cms/xmds/RegisterDisplay.hpp"
#include "cms/xmds/RequiredFiles.hpp"
#include "cms/xmds/Schedule.hpp"
#include "cms/xmds/SubmitLog.hpp"
#include "cms/xmds/SubmitStats.hpp"
#include "networking/ResponseResult.hpp"

#include "common/JoinableThread.hpp"
#include "common/dt/Timer.hpp"

#include <boost/signals2/signal.hpp>

using CollectionResultCallback = std::function<void(const PlayerError&)>;
using SignalSettingsUpdated = boost::signals2::signal<void(PlayerSettings)>;
using SignalScheduleAvailable = boost::signals2::signal<void(Schedule::Result)>;
using SignalCollectionFinished = boost::signals2::signal<void(PlayerError)>;
using SignalFilesDownloaded = boost::signals2::signal<void()>;
class XmdsRequestSender;
class StatsRecorder;
class FileCache;

struct CollectionSession
{
    CollectionResultCallback callback;
};

using CollectionSessionPtr = std::shared_ptr<CollectionSession>;

class CollectionInterval
{
public:
    CollectionInterval(XmdsRequestSender& xmdsSender, StatsRecorder& statsRecorder, FileCache& fileCache);

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
    StatsRecorder& statsRecorder_;
    FileCache& fileCache_;
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
