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
    const std::string HlsType = "hls";
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

    namespace Tranisiton
    {
        const std::string FadeIn = "fadeIn";
        const std::string FadeOut = "fadeOut";
        const std::string Fly = "fly";

        const std::string N = "N";
        const std::string NE = "NE";
        const std::string E = "E";
        const std::string SE = "SE";
        const std::string S = "S";
        const std::string SW = "SW";
        const std::string W = "W";
        const std::string NW = "NW";

        const std::string InType = "transIn";
        const std::string InDirection = "transInDirection";
        const std::string InDuration = "transInDuration";

        const std::string OutType = "transOut";
        const std::string OutDirection = "transOutDirection";
        const std::string OutDuration = "transOutDuration";
    }

}
