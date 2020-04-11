#pragma once

#include "XmdsCommand.hpp"
#include "cms/xmds/GetFile.hpp"

class GetFileCommand : public XmdsCommand<GetFile>
{
    using SignalChunkReady = boost::signals2::signal<void(const std::string&)>;

public:
    static std::unique_ptr<GetFileCommand> create(const std::string& host,
                                                  const std::string& serverKey,
                                                  const std::string& hardwareKey,
                                                  int fileId,
                                                  const std::string& fileType,
                                                  size_t chunkOffset,
                                                  size_t chunkSize);

    SignalChunkReady& chunkReady();

private:
    GetFileCommand(const std::string& host,
                   const std::string& serverKey,
                   const std::string& hardwareKey,
                   int fileId,
                   const std::string& fileType,
                   size_t chunkOffset,
                   size_t chunkSize);

    void prepare(GetFile::Request& request) override;
    void process(const GetFile::Response& response) override;

private:
    SignalChunkReady chunkReady_;
    int fileId_;
    std::string fileType_;
    size_t chunkOffset_;
    size_t chunkSize_;
};
