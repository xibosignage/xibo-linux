#pragma once

#include "RequiredFilesDownloader.hpp"
#include "xmds/RegisterDisplay.hpp"
#include "xmds/RequiredFiles.hpp"
#include "utils/ITimerProvider.hpp"

struct CollectionResult
{
    bool success = false;
};

using CollectionResultCallback = std::function<void(const CollectionResult&)>;

struct CollectionSession
{
    size_t requestsExecuted = 0;
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
    void onCollectionFinished(const CollectionResult& result);

    void onDisplayRegistered(const RegisterDisplay::Response::Status& status, const PlayerSettings& settings, CollectionSessionPtr session);
    void onRequiredFiles(const RegularFiles& files, const ResourceFiles& resources, CollectionSessionPtr session);

private:
    int m_collectInterval;
    RequiredFilesDownloader m_downloader;
    std::unique_ptr<ITimerProvider> m_intervalTimer;

};
