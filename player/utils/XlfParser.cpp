#include "XlfParser.hpp"
#include <regex>

XlfParser::XlfParser(const std::string& full_path)
{
    spdlog::get(LOGGER)->debug(full_path);

    boost::property_tree::read_xml(full_path, m_tree);
}

Params XlfParser::parse_layout_params(const xlf_node& layout_node)
{
    spdlog::get(LOGGER)->debug("parse layout");
    boost::property_tree::ptree layout_params;

    auto attrs = layout_node.get_child("<xmlattr>");
    layout_params.put("schemaVersion", attrs.get<int>("schemaVersion"));
    layout_params.put("width", attrs.get<int>("width"));
    layout_params.put("height", attrs.get<int>("height"));
    layout_params.put("background", attrs.get_optional<std::string>("background").value_or(std::string{}));
    layout_params.put("bgcolor", attrs.get_optional<std::string>("bgcolor").value_or(std::string{}));

    return layout_params;
}

Params XlfParser::parse_region_params(const xlf_node& region_node)
{
    spdlog::get(LOGGER)->debug("parse region");
    boost::property_tree::ptree region_params;

    static int available_zindex = 0;
    auto attrs = region_node.get_child("<xmlattr>");
    auto options = region_node.get_child("options");
    int id = attrs.get<int>("id");

    region_params.put("id", id);
    region_params.put("width", static_cast<int>(attrs.get<float>("width")));
    region_params.put("height", static_cast<int>(attrs.get<float>("height")));
    region_params.put("top", static_cast<int>(attrs.get<float>("top")));
    region_params.put("left", static_cast<int>(attrs.get<float>("left")));

    auto zindex_optional = attrs.get_optional<int>("zindex");
    int zindex = zindex_optional ? zindex_optional.value() : available_zindex++;
    region_params.put("zindex", zindex);
    region_params.put("loop", options.get_optional<bool>("loop").value_or(false));

    return region_params;
}

Params XlfParser::parse_layout()
{
    auto layout_node = m_tree.get_child("layout");
    auto layout_params = parse_layout_params(layout_node);
    auto& regions = layout_params.add_child("regions", {});

    for(auto [layout_node_name, region_node] : layout_node)
    {
        if(layout_node_name == "region")
        {
            auto current_region = parse_region(region_node);
            regions.add_child(std::to_string(current_region.get<int>("id")), current_region);
        }
    }

    return layout_params;
}

Params XlfParser::parse_region(const xlf_node& region_node)
{
    auto region_params = parse_region_params(region_node);
    auto& media = region_params.add_child("media", {});

    for(auto [region_node_name, media_node]: region_node)
    {
        if(region_node_name == "media")
        {
            auto current_media = parse_media(media_node);
            media.add_child(std::to_string(current_media.get<int>("id")), current_media);
        }
    }
    return region_params;
}

Params XlfParser::parse_media(const xlf_node& media_node)
{
    auto type = media_node.get_child("<xmlattr>").get<std::string>("type");
    auto render = media_node.get_child("<xmlattr>").get<std::string>("render");

    if(type == "image")
        return parse_media_params<MediaType::Image>(media_node);
    else if(type == "video")
        return parse_media_params<MediaType::Video>(media_node);
    else if(type == "audio")
        return parse_media_params<MediaType::Audio>(media_node);
    // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
    else if(render == "html" || type == "text" || type == "ticker" || type == "datasetview" || type == "embedded")
        return parse_media_params<MediaType::WebView>(media_node);

    return {};
}

// FIXME temporary workaround
std::string XlfParser::get_path(int id, const boost::optional<std::string>& uri, const std::string& type)
{
    if(!uri || type == "ticker" || type == "forecastio")
    {
        boost::property_tree::ptree tree;
        boost::property_tree::read_xml(XiboApp::example_dir() + "/requiredFiles.xml", tree);

        auto required_files = tree.get_child("RequiredFiles").get_child("RequiredFileList");
        for(auto&& required_file : required_files)
        {
            auto file_info = required_file.second;
            if(file_info.get<std::string>("FileType") == "resource" && file_info.get<int>("MediaId") == id)
            {
                return file_info.get<std::string>("Path");
            }
        }
        return std::string{};
    }
    return uri.value();
}

boost::optional<int> XlfParser::parse_duration(const std::string& path)
{
    std::ifstream in(path);
    std::string line;
    std::regex re("<!-- DURATION=([0-9]+) -->");
    while(std::getline(in, line))
    {
        std::smatch match;
        if(std::regex_search(line, match, re) && match.size() > 1)
        {
            spdlog::get(LOGGER)->debug("DURATION parsed");
            // NOTE: 0 for full match, 1 for the first group match
            return std::stoi(match[1].str());
        }
    }
    return boost::optional<int>{};
}
