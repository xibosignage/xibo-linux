#include "MainController.hpp"

#include "control/MainLayout.hpp"
#include "control/MediaContainer.hpp"

#include "factories/MainLayoutFactory.hpp"
#include "factories/MediaContainerFactory.hpp"
#include "factories/ImageFactory.hpp"
#include "factories/VideoFactory.hpp"
#include "factories/AudioFactory.hpp"
#include "factories/WebViewFactory.hpp"

#include "media/GetMediaPosition.hpp"
#include "parsers/Resources.hpp"

#include <boost/property_tree/xml_parser.hpp>

std::unique_ptr<IMainLayout> MainBuilder::buildLayoutWithChildren(const xlf_node& tree)
{
    return buildLayout(tree.get_child(Resources::Layout::NodeName));
}

std::unique_ptr<IMainLayout> MainBuilder::buildLayout(const xlf_node& layoutNode)
{
    MainLayoutFactory layoutFactory(layoutNode);
    auto layout = layoutFactory.create();
    for(auto [nodeName, containerNode] : layoutNode)
    {
        if(nodeName == Resources::Region::NodeName)
        {
            int x = static_cast<int>(containerNode.get<float>("<xmlattr>.left"));
            int y = static_cast<int>(containerNode.get<float>("<xmlattr>.top"));
            layout->addMediaContainer(createContainer(containerNode), x, y);
        }
    }
    return layout;
}

std::unique_ptr<IMediaContainer> MainBuilder::createContainer(const xlf_node& containerNode)
{
    MediaContainerFactory containterFactory(containerNode);
    auto mediaContainer = containterFactory.create();
    for(auto [nodeName, mediaNode] : containerNode)
    {
        if(nodeName == Resources::Media::NodeName)
        {
            createAndAddMediaToContainer(*mediaContainer, mediaNode);
        }
    }
    return mediaContainer;
}

void MainBuilder::createAndAddMediaToContainer(IMediaContainer& mediaController,
                                                  const xlf_node& mediaNode)
{
    auto type = mediaNode.get<std::string>(Resources::Attrs + "." + Resources::Media::Type);

    if(type != Resources::Media::AudioType)
    {
        auto visibleMedia = createVisibleMedia(type, mediaNode);
        GetMediaPosition visitor(mediaController.width(), mediaController.height());
        visibleMedia->apply(visitor);

        mediaController.addMedia(std::move(visibleMedia), visitor.getMediaX(), visitor.getMediaY());
    }
    else
    {
        mediaController.addMedia(createInvisibleMedia(type, mediaNode));
    }
}

std::unique_ptr<IMedia> MainBuilder::createVisibleMedia(const std::string& type,
                                                           const xlf_node& mediaNode)
{
    std::unique_ptr<MediaFactory> factory;

    if(type == Resources::Media::ImageType)
        factory = std::make_unique<ImageFactory>(mediaNode);
    else if(type == Resources::Media::VideoType)
        factory = std::make_unique<VideoFactory>(mediaNode);
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        factory = std::make_unique<WebViewFactory>(mediaNode);

    return factory->create();
}

std::unique_ptr<IMedia> MainBuilder::createInvisibleMedia(const std::string& type,
                                                             const xlf_node& mediaNode)
{
    if(type == Resources::Media::AudioType)
        return std::make_unique<AudioFactory>(mediaNode)->create();
    else
        return nullptr;
}
