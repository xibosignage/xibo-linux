#include "GetResourceCommand.hpp"

GetResourceCommand::GetResourceCommand(const std::string& host,
                                       const std::string& serverKey,
                                       const std::string& hardwareKey,
                                       int layoutId,
                                       int regionId,
                                       int mediaId) :
    XmdsCommand(host, serverKey, hardwareKey),
    layoutId_(layoutId),
    regionId_(regionId),
    mediaId_(mediaId)
{
}

std::unique_ptr<GetResourceCommand> GetResourceCommand::create(const std::string& host,
                                                               const std::string& serverKey,
                                                               const std::string& hardwareKey,
                                                               int layoutId,
                                                               int regionId,
                                                               int mediaId)
{
    return std::unique_ptr<GetResourceCommand>(
        new GetResourceCommand(host, serverKey, hardwareKey, layoutId, regionId, mediaId));
}

GetResourceCommand::SignalResourceReady& GetResourceCommand::resourceReady()
{
    return resourceReady_;
}

void GetResourceCommand::prepare(GetResource::Request& request)
{
    request.layoutId = layoutId_;
    request.regionId = std::to_string(regionId_);
    request.mediaId = std::to_string(mediaId_);
}

void GetResourceCommand::process(const GetResource::Response& response)
{
    resourceReady_(response.resource);
}
