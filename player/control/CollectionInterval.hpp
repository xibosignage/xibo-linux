#ifndef COLLECTIONINTERVAL_HPP
#define COLLECTIONINTERVAL_HPP

#include "xmds/XMDSManager.hpp"

#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <spdlog/spdlog.h>

struct RequiredFilesSession
{
    size_t download_count = 0;
    size_t download_overall = 0;
};

using RequiredFilesSessionPtr = std::shared_ptr<RequiredFilesSession>;

class CollectionInterval
{
public:
    CollectionInterval();
    void start();
    sigc::signal<void>& signal_finished();
    sigc::signal<void, PlayerSettings>& signal_settings_updated();

private:
    void collect_data();
    void update_timer(uint collect_interval);

private:
    void on_register_display(const RegisterDisplay::Response& response);
    void on_required_files(const RequiredFiles::Response& response);

    void download_callback(const std::string& filename, RequiredFilesSessionPtr session);

private:
    std::shared_ptr<spdlog::logger> m_logger;
    sigc::signal<void> m_signal_finished;
    sigc::signal<void, PlayerSettings> m_signal_settings_updated;
    sigc::connection m_interval_connection;
    boost::optional<uint> m_collect_interval;

};

#endif // COLLECTIONINTERVAL_HPP
