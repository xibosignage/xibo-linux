#include "GetFileCommand.hpp"

GetFileCommand::GetFileCommand(const std::string& host,
                               const std::string& serverKey,
                               const std::string& hardwareKey,
                               int fileId,
                               const std::string& fileType,
                               size_t chunkOffset,
                               size_t chunkSize) :
    XmdsCommand(host, serverKey, hardwareKey),
    fileId_(fileId),
    fileType_(fileType),
    chunkOffset_(chunkOffset),
    chunkSize_(chunkSize)
{
}

std::unique_ptr<GetFileCommand> GetFileCommand::create(const std::string& host,
                                                       const std::string& serverKey,
                                                       const std::string& hardwareKey,
                                                       int fileId,
                                                       const std::string& fileType,
                                                       size_t chunkOffset,
                                                       size_t chunkSize)
{
    return std::unique_ptr<GetFileCommand>(
        new GetFileCommand(host, serverKey, hardwareKey, fileId, fileType, chunkOffset, chunkSize));
}

GetFileCommand::SignalChunkReady& GetFileCommand::chunkReady()
{
    return chunkReady_;
}

void GetFileCommand::prepare(GetFile::Request& request)
{
    request.fileId = std::to_string(fileId_);
    request.fileType = fileType_;
    request.chunkOffset = chunkOffset_;
    request.chunkSize = chunkSize_;
}

void GetFileCommand::process(const GetFile::Response& response)
{
    chunkReady_(response.base64chunk);
}
