#include <string>

namespace ResourcesXlf
{
    const std::string Attrs = "<xmlattr>";
    const std::string Options = "options";

    std::string attr(const std::string& property);
    std::string option(const std::string& property);

    const std::string LayoutNode = "layout";
    const std::string RegionNode = "region";
    const std::string MediaNode = "media";

    const std::string AudioType = "audio";
    const std::string VideoType = "video";
    const std::string ImageType = "image";


    namespace Layout
    {
        const std::string Width = "width";
        const std::string Height = "height";
        const std::string BackgroundPath = "background";
        const std::string BackgroundColor = "bgcolor";
    }

    namespace Region
    {
        const std::string Width = "width";
        const std::string Height = "height";
        const std::string Left = "left";
        const std::string Top = "top";
        const std::string Zindex = "zindex";
        const std::string Loop = "loop";
    }

    namespace Media
    {
        const std::string Id = "id";
        const std::string Type = "type";
        const std::string Uri = "uri";
        const std::string Duration = "duration";

        namespace Audio
        {
            const std::string Mute = "mute";
            const std::string Loop = "loop";
            const std::string Volume = "volume";
        }

        namespace Video
        {
            const std::string Mute = "mute";
            const std::string Loop = "loop";
        }

        namespace WebView
        {
            const std::string Transparent = "transparency";
        }

        namespace Image
        {
            const std::string ScaleType = "scaleType";
            const std::string Align = "scaleType";
            const std::string Valign = "scaleType";
        }
    }

}
