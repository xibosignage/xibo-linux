#pragma once

#include "common/Parsing.hpp"
#include "control/common/XlfResources.hpp"

namespace XlfResources::MainLayout
{
const std::string SchemaVersion = Parsing::xmlAttr("schemaVersion");
const std::string Width = Parsing::xmlAttr("width");
const std::string Height = Parsing::xmlAttr("height");
const std::string BackgroundPath = Parsing::xmlAttr("background");
const std::string BackgroundColor = Parsing::xmlAttr("bgcolor");
}
