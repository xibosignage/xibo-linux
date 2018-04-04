#include "utilities.hpp"
#include "constants.hpp"
#include "XiboApp.hpp"

#include <stdexcept>
#include <spdlog/spdlog.h>

uint32_t utilities::to_hex(const std::string& str_color)
{
    // remove '#' sign at the beginning
    std::string str_hex = str_color.substr(1);

    // convert 3-digit hex to 6-digit hex
    if(str_hex.size() == 3)
        str_hex = std::string(2, str_hex[0]) + std::string(2, str_hex[1]) + std::string(2, str_hex[2]);
    // add default alpha channel
    if(str_hex.size() == 6)
        str_hex += "FF";

    return static_cast<uint32_t>(std::stoul(str_hex, nullptr, 16));
}

const std::string& utilities::example_dir()
{
    return XiboApp::app().command_line_parser().example_dir();
}

const std::string& utilities::xlf_file()
{
    return XiboApp::app().command_line_parser().xlf_file();
}
const XlfParser& utilities::xlf_parser()
{
    return XiboApp::app().xlf_parser();
}
