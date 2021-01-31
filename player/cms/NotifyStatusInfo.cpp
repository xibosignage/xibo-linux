#include "NotifyStatusInfo.hpp"

#include "common/parsing/Parsing.hpp"

std::string NotifyStatusInfo::string() const
{
    JsonNode tree;
    tree.put("currentLayoutId", currentLayoutId);
    tree.put("availableSpace", spaceUsageInfo.available);
    tree.put("totalSpace", spaceUsageInfo.total);
    //    tree.put("lastCommandSuccess", ""); TODO: implement when commands will be available
    tree.put("deviceName", static_cast<std::string>(deviceName));
    tree.put("timeZone", timezone);
    return Parsing::jsonToString(tree);
}
