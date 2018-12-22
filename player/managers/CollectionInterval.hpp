#pragma once

#include "RequiredFilesDownloader.hpp"
#include "RequiredResourcesDownloader.hpp"

#include "xmds/RegisterDisplay.hpp"
#include "xmds/RequiredFiles.hpp"
#include "utils/ITimerProvider.hpp"
#include "utils/AsyncListener.hpp"

struct CollectionResult
{
    bool success = false;
};

using CollectionResultCallback = std::function<void(const CollectionResult&)>;

struct CollectionSession
{
    RequiredFilesDownloader filesDownloader;
    RequiredResourcesDownloader resourcesDownloader;
    std::shared_ptr<AsyncListener> listener;
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

    void onDisplayRegistered(const RegisterDisplay::Response::Status& status, const PlayerSettings& settings, CollectionSessionPtr session);
    void onRequiredFiles(const RegularFiles& files, const ResourceFiles& resources, CollectionSessionPtr session);

private:
    int m_collectInterval;
    std::unique_ptr<ITimerProvider> m_intervalTimer;

};
