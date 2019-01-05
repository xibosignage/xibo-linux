#pragma once

#include "RequiredFilesDownloader.hpp"
#include "RequiredResourcesDownloader.hpp"

#include "xmds/RegisterDisplay.hpp"
#include "xmds/RequiredFiles.hpp"
#include "xmds/Schedule.hpp"

#include "utils/ITimerProvider.hpp"

struct CollectionResult
{
    bool success = false;
};

using CollectionResultCallback = std::function<void(const CollectionResult&)>;

struct CollectionSession
{
    CollectionResultCallback callback;
};

using CollectionSessionPtr = std::shared_ptr<CollectionSession>;

class CollectionInterval
{
public:
    CollectionInterval();

    void start();
    void collect(CollectionResultCallback callback);

private:
    void startTimer();
    void updateTimer(int collectInterval);

    void startRegularCollection();
    void sessionFinished(CollectionSessionPtr session);
    void onCollectionFinished(const CollectionResult& result);

    void onDisplayRegistered(const RegisterDisplay::Response& response, CollectionSessionPtr session);
    void displayMessage(const RegisterDisplay::Response::Status& status);
    void onRequiredFiles(const RequiredFiles::Response& response, CollectionSessionPtr session);
    void onSchedule(const Schedule::Response& response, CollectionSessionPtr session);

private:
    int m_collectInterval;
    std::unique_ptr<ITimerProvider> m_intervalTimer;

};
