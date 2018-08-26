#ifndef COLLECTIONINTERVAL_HPP
#define COLLECTIONINTERVAL_HPP

#include "xmds/XMDSManager.hpp"

#include <sigc++/signal.h>
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

private:
    void collect_data();

private:
    void on_register_display(const RegisterDisplay::Response& response);
    void on_required_files(const RequiredFiles::Response& response);

    void download_callback(const std::string& filename, RequiredFilesSessionPtr session);

private:
    std::shared_ptr<spdlog::logger> m_logger;
    sigc::signal<void> m_signal_finished;

};

#endif // COLLECTIONINTERVAL_HPP
