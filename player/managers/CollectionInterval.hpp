#ifndef COLLECTIONINTERVAL_HPP
#define COLLECTIONINTERVAL_HPP

#include "xmds/RegisterDisplay.hpp"
#include "xmds/RequiredFiles.hpp"

#include <sigc++/signal.h>
#include <sigc++/connection.h>

struct RequiredFilesSession
{
    size_t downloadCount = 0;
    size_t downloadOverall = 0;
};

using RequiredFilesSessionPtr = std::shared_ptr<RequiredFilesSession>;

class CollectionInterval
{
public:
    CollectionInterval();
    void start();
    sigc::signal<void>& signalFinished();
    sigc::signal<void, PlayerSettings>& signalSettingsUpdated();

private:
    void collectData();
    void updateTimer(uint collectInterval);

private:
    void onRegisterDisplay(const RegisterDisplay::Response& response);
    void onRequiredFiles(const RequiredFiles::Response& response);

    void downloadCallback(const std::string& filename, RequiredFilesSessionPtr session);

private:
    sigc::signal<void> m_signalFinished;
    sigc::signal<void, PlayerSettings> m_signalSettingsUpdated;
    sigc::connection m_intervalConnection;
    boost::optional<uint> m_collectInterval;

};

#endif // COLLECTIONINTERVAL_HPP
