#pragma once

#include "SubmitXmdsCommand.hpp"
#include "cms/xmds/MediaInventory.hpp"

class StatsRecorder;

class MediaInventoryCommand : public SubmitXmdsCommand<MediaInventory>
{
public:
    static std::unique_ptr<MediaInventoryCommand> create(const std::string& host,
                                                         const std::string& serverKey,
                                                         const std::string& hardwareKey,
                                                         MediaInventoryItems&& items);

private:
    MediaInventoryCommand(const std::string& host,
                          const std::string& serverKey,
                          const std::string& hardwareKey,
                          MediaInventoryItems&& items);

    void prepare(MediaInventory::Request& request) override;

private:
    MediaInventoryItems items_;
};
