#include "MainParser.hpp"

#include "XlfResources.hpp"

#include "utils/Utilities.hpp"
#include "utils/Resources.hpp"

#include "parsers/MainLayoutParser.hpp"
#include "parsers/RegionsParser.hpp"
#include "parsers/ImageParser.hpp"
#include "parsers/WebViewParser.hpp"
#include "parsers/AudioParser.hpp"
#include "parsers/VideoParser.hpp"

#include "model/AudioModel.hpp"
#include "model/VideoModel.hpp"
#include "model/WebViewModel.hpp"
#include "model/ImageModel.hpp"

std::shared_ptr<MainLayoutModel> MainParser::parseLayoutModel(int layoutId)
{
    auto tree = Utils::parseXmlFromPath(getXlfFilePath(layoutId));
    auto layoutNode = tree.get_child(ResourcesXlf::LayoutNode);

    return parseLayout(layoutNode);
}

FilePath MainParser::getXlfFilePath(int layoutId)
{
    auto xlfFile = std::to_string(layoutId) + ".xlf";
    return Resources::directory() / xlfFile;
}

std::shared_ptr<MainLayoutModel> MainParser::parseLayout(const xml_node& layoutNode)
{
    MainLayoutParser parser{layoutNode};
    auto layout = std::make_shared<MainLayoutModel>(parser.parse());

    for(auto [nodeName, node] : layoutNode)
    {
        if(nodeName == ResourcesXlf::RegionNode)
        {
            layout->addRegion(parseRegion(node));
        }
    }

    return layout;
}

std::shared_ptr<RegionModel> MainParser::parseRegion(const xml_node& regionNode)
{
    RegionsParser parser{regionNode};
    auto region = std::make_shared<RegionModel>(parser.parse());

    for(auto [nodeName, node] : regionNode)
    {
        if(nodeName == ResourcesXlf::MediaNode)
        {
            region->addMedia(parseMedia(node));
        }
    }

    return region;
}

std::shared_ptr<MediaModel> MainParser::parseMedia(const xml_node& node)
{
    auto type = node.get<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Type));
    std::shared_ptr<MediaModel> media;

    if(type == ResourcesXlf::ImageType)
        media = parseImage(node);
    else if(type == ResourcesXlf::AudioType)
        media = parseAudio(node);
    else if(type == ResourcesXlf::VideoType)
        media = parseVideo(node);
    else
        media = parseWebView(node);

    attachAdditionalMedia(media, node);

    return media;
}

std::shared_ptr<MediaModel> MainParser::parseImage(const xml_node& node)
{
    ImageParser parser{node};
    return std::make_shared<ImageModel>(parser.parse());
}

std::shared_ptr<MediaModel> MainParser::parseAudio(const xml_node& node)
{
    AudioParser parser{node};
    return std::make_shared<AudioModel>(parser.parse());
}

std::shared_ptr<MediaModel> MainParser::parseVideo(const xml_node& node)
{
    VideoParser parser{node};
    return std::make_shared<VideoModel>(parser.parse());
}

std::shared_ptr<MediaModel> MainParser::parseWebView(const xml_node& node)
{
    WebViewParser parser{node};
    return std::make_shared<WebViewModel>(parser.parse());
}

void MainParser::attachAdditionalMedia(const std::shared_ptr<MediaModel>& media, const xml_node& node)
{
    for(auto [nodeName, attachedNode] : node)
    {
        if(nodeName == ResourcesXlf::AudioType)
        {
            AudioParser parser{attachedNode};
            media->attachAudio(std::make_shared<AudioModel>(parser.parseNode()));
        }
    }
}
