#pragma once

#include <string_view>
#include <string>

namespace XMDSResources
{
    namespace GetResource
    {
        const std::string_view RequestName = "GetResource";

        const std::string Resource = "resource";
    }

    namespace RegisterDisplay
    {
        const std::string_view RequestName = "RegisterDisplay";

        const std::string ActivationMessage = "ActivationMessage";
        const std::string Display = "display";
        const std::string DisplayAttrs = "<xmlattr>";
        const std::string Status = "status";
        const std::string StatusMessage = "message";

        namespace Settings
        {
            const std::string CollectInterval = "collectInterval";
            const std::string DownloadStartWindow = "downloadStartWindow";
            const std::string DownloadEndWindow = "downloadEndWindow";
            const std::string StatsEnabled = "statsEnabled";
            const std::string XmrNetworkAddress = "xmrNetworkAddress";
            const std::string SizeX = "sizeX";
            const std::string SizeY = "sizeY";
            const std::string OffsetX = "offsetX";
            const std::string OffsetY = "offsetY";
            const std::string LogLevel = "logLevel";
            const std::string EnableShellCommands = "enableShellCommands";
            const std::string ExpireModifiedLayouts = "expireModifiedLayouts";
            const std::string MaxConcurrentDownloads = "maxConcurrentDownloads";
            const std::string SendCurrentLayoutAsStatusUpdate = "sendCurrentLayoutAsStatusUpdate";
            const std::string ScreenShotRequestInterval = "screenShotRequestInterval";
            const std::string ScreenShotSize = "screenShotSize";
            const std::string MaxLogFileUploads = "maxLogFileUploads";
            const std::string EmbeddedServerPort = "embeddedServerPort";
            const std::string PreventSleep = "preventSleep";
            const std::string DisplayName = "displayName";
            const std::string ScreenShotRequested = "screenShotRequested";
        }
    }

    namespace RequiredFiles
    {
        const std::string_view RequestName = "RequiredFiles";

        const std::string RequiredFilesXml = "RequiredFilesXml";
        const std::string Files = "files";
        const std::string File = "file";
        const std::string FileAttrs = "<xmlattr>";
        const std::string FileType = "type";
        const std::string Media = "media";
        const std::string Layout = "layout";
        const std::string Resource = "resource";

        namespace RegularFile
        {
            const std::string Id = "id";
            const std::string Size = "size";
            const std::string MD5 = "md5";
            const std::string Path = "path";
            const std::string SaveAs = "saveAs";
            const std::string DownloadType = "download";
            const std::string HTTPDownload = "http";
            const std::string XMDSDownload = "xmds";

        }

        namespace ResourceFile
        {
            const std::string LayoutId = "layoutid";
            const std::string RegionId = "regionid";
            const std::string MediaId = "mediaid";
        }
    }
}
