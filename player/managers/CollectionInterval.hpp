#pragma once

#include "RequiredFilesDownloader.hpp"
#include "RequiredResourcesDownloader.hpp"
#include "CollectionResult.hpp"

#include "xmds/RegisterDisplay.hpp"
#include "xmds/RequiredFiles.hpp"

#include "utils/ITimerProvider.hpp"
#include "utils/JoinableThread.hpp"
#include "utils/ResponseResult.hpp"
#include "events/Observable.hpp"

using CollectionResultCallback = std::function<void(const CollectionResult&)>;

struct CollectionSession
{
    CollectionResult result;
    CollectionResultCallback callback;
};

using CollectionSessionPtr = std::shared_ptr<CollectionSession>;

class CollectionInterval : public Observable<>
{
public:
    CollectionInterval();

    void startRegularCollection();
    void collectOnce(CollectionResultCallback callback);

private:
    void startTimer();
    void updateTimer(int collectInterval);

    void sessionFinished(CollectionSessionPtr session, PlayerError = {});
    void onRegularCollectionFinished(const CollectionResult& result);

    void onDisplayRegistered(const ResponseResult<RegisterDisplay::Result>& registerDisplay, CollectionSessionPtr session);
    void displayMessage(const RegisterDisplay::Result::Status& status);
    void onRequiredFiles(const ResponseResult<RequiredFiles::Result>& requiredFiles, CollectionSessionPtr session);
    void onSchedule(const ResponseResult<Schedule::Result>& schedule, CollectionSessionPtr session);

private:
    std::unique_ptr<JoinableThread> m_workerThread;
    int m_collectInterval;
    std::unique_ptr<ITimerProvider> m_intervalTimer;

};
