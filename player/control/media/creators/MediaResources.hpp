#pragma once

#include "control/common/XlfResources.hpp"

namespace ResourcesXlf::Media
{
    const std::string Id = "id";
    const std::string Type = "type";
    const std::string Render = "render";
    const std::string NativeRender = "native";
    const std::string HtmlRender = "html";
    const std::string Uri = "uri";
    const std::string Duration = "duration";

    const std::string AudioType = "audio";
    const std::string VideoType = "video";
    const std::string LocalVideoType = "localvideo";
    const std::string ImageType = "image";
    const std::string TextType = "text";
    const std::string EmbeddedType = "embedded";
    const std::string TickerType = "ticker";
    const std::string WebpageType = "webpage";
    const std::string DatasetViewType = "datasetview";

    namespace Geometry
    {
        const std::string ScaleType = "scaleType";
        const std::string Scaled = "center";
        const std::string Aspect = "aspect";
        const std::string Stretch = "stretch";

        const std::string Align = "align";
        const std::string RightAlign = "right";
        const std::string CenterAlign = "center";
        const std::string LeftAlign = "left";

        const std::string Valign = "valign";
        const std::string TopValign = "top";
        const std::string MiddleValign = "middle";
        const std::string BottomValign = "bottom";
    }

}
