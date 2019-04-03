#pragma once

#include "RequiredFilesDownloader.hpp"

#include "xmds/RegisterDisplay.hpp"
#include "xmds/RequiredFiles.hpp"
#include "xmds/Schedule.hpp"
#include "xmds/SubmitLog.hpp"

#include "utils/ITimerProvider.hpp"
#include "utils/JoinableThread.hpp"
#include "utils/ResponseResult.hpp"

#include <sigc++/signal.h>

using CollectionResultCallback = std::function<void(const PlayerError&)>;
using SignalSettingsUpdated = sigc::signal<void(const PlayerSettings&)>;
using SignalScheduleUpdated = sigc::signal<void(const LayoutSchedule&)>;
using SignalCollectionFinished = sigc::signal<void(const PlayerError&)>;
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
    void collectOnce(CollectionResultCallback callback);
    void updateInterval(int collectInterval);

    SignalSettingsUpdated settingsUpdated();
    SignalScheduleUpdated scheduleUpdated();
    SignalCollectionFinished collectionFinished();

private:
    void startTimer();

    void sessionFinished(CollectionSessionPtr session, PlayerError = {});
    void onRegularCollectionFinished(const PlayerError& error);

    void onDisplayRegistered(const ResponseResult<RegisterDisplay::Result>& registerDisplay, CollectionSessionPtr session);
    void displayMessage(const RegisterDisplay::Result::Status& status);
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
    SignalSettingsUpdated m_settingsUpdated;
    SignalScheduleUpdated m_scheduleUpdated;
    SignalCollectionFinished m_collectionFinished;
};
