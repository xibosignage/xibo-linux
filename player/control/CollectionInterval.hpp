#ifndef COLLECTIONINTERVAL_HPP
#define COLLECTIONINTERVAL_HPP

#include "DownloadManager.hpp"
#include "xmds/XMDSManager.hpp"

class CollectionInterval
{
public:
    CollectionInterval();
    void start();

private:
    void collect_data();

private:
    void on_register_display(const RegisterDisplay::Response& response);
    void on_required_files(const RequiredFiles::Response& response);


private:
    std::shared_ptr<spdlog::logger> m_logger;
    DownloadManager m_download_manager;

};

#endif // COLLECTIONINTERVAL_HPP
