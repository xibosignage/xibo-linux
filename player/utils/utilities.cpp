#include "utilities.hpp"
#include "constants.hpp"
#include "XiboApp.hpp"

#include "parsers/ImageParser.hpp"
#include "parsers/VideoParser.hpp"
#include "parsers/AudioParser.hpp"
#include "parsers/WebViewParser.hpp"

#include <stdexcept>
#include <spdlog/spdlog.h>
#include <boost/filesystem/operations.hpp>

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

std::string utilities::example_dir()
{
    return XiboApp::app().command_line_parser().example_dir_path();
}

std::string utilities::xlf_file()
{
    return XiboApp::app().command_line_parser().xlf_path();
}

std::string utilities::app_current_dir()
{
    return boost::filesystem::current_path().string();
}

std::unique_ptr<MediaParser> utilities::get_media_parser(const xlf_node& parent_node, const xlf_node& media_node)
{
    auto type = media_node.get_child("<xmlattr>").get<std::string>("type");

    if(type == "image")
        return std::make_unique<ImageParser>(parent_node, media_node);
    else if(type == "video")
        return std::make_unique<VideoParser>(parent_node, media_node);
    else if(type == "audio")
        return std::make_unique<AudioParser>(parent_node, media_node);
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        return std::make_unique<WebViewParser>(parent_node, media_node);
}
