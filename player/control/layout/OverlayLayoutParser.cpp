#include "OverlayLayoutParser.hpp"

#include "control/common/OverlayLayout.hpp"
#include "common/FilePath.hpp"

std::shared_ptr<IOverlayLayout> OverlayLayoutParser::createView(const LayoutOptions& options)
{
    return std::make_shared<OverlayLayout>(options.width, options.height);
}
