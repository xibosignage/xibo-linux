#pragma once

#include "RequiredFilesDownloader.hpp"

#include "networking/xmds/RegisterDisplay.hpp"
#include "networking/xmds/RequiredFiles.hpp"
#include "networking/xmds/Schedule.hpp"
#include "networking/xmds/SubmitLog.hpp"

#include "utils/ITimerProvider.hpp"
#include "networking/ResponseResult.hpp"
#include "common/JoinableThread.hpp"
#include "common/Dispatcher.hpp"
#include "CmsStatus.hpp"

using CollectionResultCallback = std::function<void(const PlayerError&)>;
using SignalSettingsUpdated = Dispatcher<PlayerSettings>;
using SignalScheduleUpdated = Dispatcher<Schedule::Result>;
using SignalCollectionFinished = Dispatcher<PlayerError>;
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
    CmsStatus status();

    SignalSettingsUpdated& settingsUpdated();
    SignalScheduleUpdated& scheduleUpdated();
    SignalCollectionFinished& collectionFinished();

private:
    void startTimer();

    void sessionFinished(CollectionSessionPtr session, PlayerError = {});
    void onRegularCollectionFinished(const PlayerError& error);

    void onDisplayRegistered(const ResponseResult<RegisterDisplay::Result>& registerDisplay, CollectionSessionPtr session);
    PlayerError getDisplayStatus(const RegisterDisplay::Result::Status& status);
    void onRequiredFiles(const ResponseResult<RequiredFiles::Result>& requiredFiles, CollectionSessionPtr session);
    void onSchedule(const ResponseResult<Schedule::Result>& schedule, CollectionSessionPtr session);
    void updateMediaInventory(MediaInventoryItems&& items);
    void onSubmitLog(const ResponseResult<SubmitLog::Result>& requiredFiles, CollectionSessionPtr session);
    void submitScreenShot();

private:
    XmdsRequestSender& m_xmdsSender;
    std::unique_ptr<JoinableThread> m_workerThread;
    std::unique_ptr<ITimerProvider> m_intervalTimer;
    int m_collectInterval;
    bool m_started = false;
    bool m_registered = false;
    boost::posix_time::ptime m_lastChecked;
    size_t m_requiredFiles = 0;
    SignalSettingsUpdated m_settingsUpdated;
    SignalScheduleUpdated m_scheduleUpdated;
    SignalCollectionFinished m_collectionFinished;
};
