#include "GetFile.hpp"
#include "xmds.hpp"

template<>
std::string soap::requestString(const GetFile::Request& request)
{
    return createRequest<GetFile::Request>(request.serverKey, request.hardwareKey, request.fileId,
                                           request.fileType, request.chunkOffset, request.chunkSize);
}

template<>
GetFile::Response soap::createResponse(const std::string& soapResponse)
{
    GetFile::Response result;
    result.base64chunk = xmds::parseFileResponse(soapResponse);
    return result;
}
