#include "XlfParser.hpp"

XlfParser::XlfParser(const std::string& full_path)
{
    spdlog::get(LOGGER)->debug(full_path);

    boost::property_tree::read_xml(full_path, m_tree);
}

boost::property_tree::ptree XlfParser::get_layout_params() const
{
    return m_params;
}

void XlfParser::parse_layout_params(const boost::property_tree::ptree& layout_node)
{
    spdlog::get(LOGGER)->debug("parse layout");

    auto attrs = layout_node.get_child("<xmlattr>");
    m_params.put("schemaVersion", attrs.get<int>("schemaVersion"));
    m_params.put("width", attrs.get<int>("width"));
    m_params.put("height", attrs.get<int>("height"));
    m_params.put("background", attrs.get_optional<std::string>("background").value_or(std::string{}));
    m_params.put("bgcolor", attrs.get_optional<std::string>("bgcolor").value_or(std::string{}));
}

int XlfParser::parse_region_params(const boost::property_tree::ptree& region_node)
{
    spdlog::get(LOGGER)->debug("parse region");

    static int available_zindex = 0;
    auto attrs = region_node.get_child("<xmlattr>");
    auto options = region_node.get_child("options");
    int id = attrs.get<int>("id");

    auto& regions = m_params.get_child("regions");
    auto& current_region = regions.add_child(std::to_string(id), boost::property_tree::ptree{});
    current_region.add_child("medias", boost::property_tree::ptree{});

    current_region.put("id", id);
    current_region.put("width", static_cast<int>(attrs.get<float>("width")));
    current_region.put("height", static_cast<int>(attrs.get<float>("height")));
    current_region.put("top", static_cast<int>(attrs.get<float>("top")));
    current_region.put("left", static_cast<int>(attrs.get<float>("left")));

    auto zindex_optional = attrs.get_optional<int>("zindex");
    int zindex = zindex_optional ? zindex_optional.value() : available_zindex++;
    current_region.put("zindex", zindex);

    current_region.put("loop", options.get_optional<bool>("loop").value_or(false));
    current_region.put("transitionType", options.get_optional<std::string>("transitionType").value_or(""));
    current_region.put("transitionDirection", options.get_optional<std::string>("transitionDirection").value_or(""));
    current_region.put("transitionDuration", options.get_optional<int>("transitionDuration").value_or(0));

    return id;
}

void XlfParser::parse_xlf_tree()
{
    auto layout_node = m_tree.get_child("layout");
    parse_layout_params(layout_node);
    m_params.add_child("regions", boost::property_tree::ptree{});

    for(auto [layout_node_name, region_node] : layout_node)
    {
        if(layout_node_name == "region")
        {
            int region_id = parse_region_params(region_node);
            for(auto [region_node_name, media_node]: region_node)
            {
                if(region_node_name == "media")
                {
                    auto type = media_node.get_child("<xmlattr>").get<std::string>("type");
                    auto render = media_node.get_child("<xmlattr>").get<std::string>("render");

                    if(type == "image")
                        parse_media_params<Image>(region_id, media_node);
                    else if(type == "video")
                        parse_media_params<Video>(region_id, media_node);
                    // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
                    else if(render == "html" || type == "text" || type == "ticker" || type == "datasetview" || type == "embedded")
                        parse_media_params<WebView>(region_id, media_node);
                }
            }
        }
    }
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
