#pragma once

#include "control/common/XlfResources.hpp"

namespace ResourcesXlf::Media
{
    const std::string Id = ResourcesXlf::attr("id");
    const std::string Type = ResourcesXlf::attr("type");
    const std::string Render = ResourcesXlf::attr("render");
    const std::string Width = ResourcesXlf::attr("width");
    const std::string Height = ResourcesXlf::attr("height");
    const std::string Uri = ResourcesXlf::option("uri");
    const std::string Duration = ResourcesXlf::attr("duration");

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

    const std::string NativeRender = "native";
    const std::string HtmlRender = "html";

    namespace Geometry
    {
        const std::string ScaleType = ResourcesXlf::option("scaleType");
        const std::string Scaled = "center";
        const std::string Aspect = "aspect";
        const std::string Stretch = "stretch";

        const std::string Align = ResourcesXlf::option("align");
        const std::string RightAlign = "right";
        const std::string CenterAlign = "center";
        const std::string LeftAlign = "left";

        const std::string Valign = ResourcesXlf::option("valign");
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

        const std::string InType = ResourcesXlf::option("transIn");
        const std::string InDirection = ResourcesXlf::option("transInDirection");
        const std::string InDuration = ResourcesXlf::option("transInDuration");

        const std::string OutType = ResourcesXlf::option("transOut");
        const std::string OutDirection = ResourcesXlf::option("transOutDirection");
        const std::string OutDuration = ResourcesXlf::option("transOutDuration");
    }

}
