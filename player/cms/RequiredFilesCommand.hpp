#pragma once

#include "XmdsCommand.hpp"
#include "cms/xmds/RequiredFiles.hpp"

class RequiredFilesCommand : public XmdsCommand<RequiredFiles>
{
    using SignalFilesReady =
        boost::signals2::signal<void(const FilesToDownload<RegularFile>&, const FilesToDownload<ResourceFile>&)>;

public:
    static std::unique_ptr<RequiredFilesCommand> create(const std::string& host,
                                                        const std::string& serverKey,
                                                        const std::string& hardwareKey);

    SignalFilesReady& filesReady();

private:
    RequiredFilesCommand(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    void process(const RequiredFiles::Response& response) override;

private:
    SignalFilesReady filesReady_;
};
