#pragma once

#include "XmdsCommand.hpp"
#include "cms/xmds/GetResource.hpp"

class GetResourceCommand : public XmdsCommand<GetResource>
{
    using SignalResourceReady = boost::signals2::signal<void(const std::string&)>;

public:
    static std::unique_ptr<GetResourceCommand> create(const std::string& host,
                                                      const std::string& serverKey,
                                                      const std::string& hardwareKey,
                                                      int layoutId,
                                                      int regionId,
                                                      int mediaId);

    SignalResourceReady& resourceReady();

private:
    GetResourceCommand(const std::string& host,
                       const std::string& serverKey,
                       const std::string& hardwareKey,
                       int layoutId,
                       int regionId,
                       int mediaId);

    void prepare(GetResource::Request& request) override;
    void process(const GetResource::Response& response) override;

private:
    SignalResourceReady resourceReady_;
    int layoutId_;
    int regionId_;
    int mediaId_;
};
