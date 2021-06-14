#pragma once

#include "Command.hpp"
#include "CommandCode.hpp"

#include <map>
#include <memory>

using PredefinedCommands = std::map<CommandCode, std::shared_ptr<Command>>;