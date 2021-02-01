#pragma once

#include <string>
#include <string_view>

#include "common/parsing/Parsing.hpp"

namespace XmdsResources
{
    namespace GetResource
    {
        const std::string_view Name = "GetResource";

        const std::string Resource = "resource";
    }

    namespace Schedule
    {
        const std::string_view Name = "Schedule";

        const std::string ScheduleXml = "ScheduleXml";
        const std::string Schedule = "schedule";
        const std::string Layout = "layout";
        const std::string DefaultLayout = "default";
        const std::string Overlays = "overlays";
        const std::string OverlayLayout = "overlay";
        const std::string GlobalDependants = "dependants";

        const std::string Generated = Parsing::xmlAttr("generated");
        const std::string ScheduleId = Parsing::xmlAttr("scheduleid");
        const std::string Id = Parsing::xmlAttr("file");
        const std::string Priority = Parsing::xmlAttr("priority");
        const std::string StartDT = Parsing::xmlAttr("fromdt");
        const std::string EndDT = Parsing::xmlAttr("todt");
        const std::string LocalDependants = "dependents";
        const std::string DependantFile = "file";
    }

    namespace RegisterDisplay
    {
        const std::string_view Name = "RegisterDisplay";

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
            const std::string Width = "sizeX";
            const std::string Height = "sizeY";
            const std::string XPos = "offsetX";
            const std::string YPos = "offsetY";
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
        const std::string_view Name = "RequiredFiles";

        const std::string RequiredFilesXml = "RequiredFilesXml";
        const std::string Files = "files";
        const std::string File = "file";
        const std::string FileAttrs = "<xmlattr>";
        const std::string FileType = "type";
        const std::string MediaType = "media";
        const std::string LayoutType = "layout";
        const std::string ResourceType = "resource";

        namespace RegularFile
        {
            const std::string Id = "id";
            const std::string Size = "size";
            const std::string MD5 = "md5";
            const std::string Path = "path";
            const std::string Name = "saveAs";
            const std::string DownloadType = "download";
            const std::string HttpDownload = "http";
            const std::string XmdsDownload = "xmds";

        }

        namespace ResourceFile
        {
            const std::string LayoutId = "layoutid";
            const std::string RegionId = "regionid";
            const std::string MediaId = "mediaid";
            const std::string LastUpdate = "updated";
        }
    }

    namespace GetFile
    {
        const std::string_view Name = "GetFile";
        const std::string FileChunk = "file";
    }

    namespace MediaInventory
    {
        const std::string_view Name = "MediaInventory";
        const std::string Success = "success";
    }

    namespace SubmitLog
    {
        const std::string_view Name = "SubmitLog";
        const std::string Success = "success";
    }

    namespace SubmitStats
    {
        const std::string_view Name = "SubmitStats";
        const std::string Success = "success";
    }

    namespace NotifyStatus
    {
        const std::string_view Name = "NotifyStatus";
        const std::string Success = "success";
    }

    namespace SubmitScreenShot
    {
        const std::string_view Name = "SubmitScreenShot";
        const std::string Success = "success";
    }
}
