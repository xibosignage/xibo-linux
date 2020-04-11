#include "MediaInventoryCommand.hpp"

MediaInventoryCommand::MediaInventoryCommand(const std::string& host,
                                             const std::string& serverKey,
                                             const std::string& hardwareKey,
                                             MediaInventoryItems&& items) :
    SubmitXmdsCommand(host, serverKey, hardwareKey),
    items_(std::move(items))
{
}

std::unique_ptr<MediaInventoryCommand> MediaInventoryCommand::create(const std::string& host,
                                                                     const std::string& serverKey,
                                                                     const std::string& hardwareKey,
                                                                     MediaInventoryItems&& items)
{
    return std::unique_ptr<MediaInventoryCommand>(
        new MediaInventoryCommand(host, serverKey, hardwareKey, std::move(items)));
}

void MediaInventoryCommand::prepare(MediaInventory::Request& request)
{
    request.inventory = std::move(items_);
}
