#ifndef PLAYERSETTINGS_HPP
#define PLAYERSETTINGS_HPP

#include <string>

struct PlayerSettings
{
    int collect_interval;
    std::string download_start_window;
    std::string download_end_window;
    bool stats_enabled;
    std::string xmr_network_address;
    double sizeX;
    double sizeY;
    double offsetX;
    double offsetY;
    std::string log_level;
    bool shell_commands_enabled;
    bool modified_layouts_enabled;
    int max_concurrent_downloads;
    bool status_layout_update;
    int screenshot_interval;
    int screenshot_size;
    int max_log_files_uploads;
    int embedded_server_port;
    bool prevent_sleep;
    std::string display_name;
    bool screenshot_requested;
};

#endif // PLAYERSETTINGS_HPP
