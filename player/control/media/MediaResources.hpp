#pragma once

#include "common/Parsing.hpp"
#include "control/common/XlfResources.hpp"

namespace XlfResources::Media
{
    const std::string Id = Parsing::xmlAttr("id");
    const std::string Type = Parsing::xmlAttr("type");
    const std::string Render = Parsing::xmlAttr("render");
    const std::string Width = Parsing::xmlAttr("width");
    const std::string Height = Parsing::xmlAttr("height");
    const std::string Uri = XlfResources::option("uri");
    const std::string Duration = Parsing::xmlAttr("duration");

    const std::string AudioType = "audio";
    const std::string AudioNodeType = "audionode";
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
        const std::string ScaleType = XlfResources::option("scaleType");
        const std::string Scaled = "center";
        const std::string Aspect = "aspect";
        const std::string Stretch = "stretch";

        const std::string Align = XlfResources::option("align");
        const std::string RightAlign = "right";
        const std::string CenterAlign = "center";
        const std::string LeftAlign = "left";

        const std::string Valign = XlfResources::option("valign");
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

        const std::string InType = XlfResources::option("transIn");
        const std::string InDirection = XlfResources::option("transInDirection");
        const std::string InDuration = XlfResources::option("transInDuration");

        const std::string OutType = XlfResources::option("transOut");
        const std::string OutDirection = XlfResources::option("transOutDirection");
        const std::string OutDuration = XlfResources::option("transOutDuration");
    }

}
