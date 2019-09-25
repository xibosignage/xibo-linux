#include "OverlayLayoutParser.hpp"

#include "common/fs/FilePath.hpp"
#include "control/common/OverlayLayout.hpp"

std::shared_ptr<IOverlayLayout> OverlayLayoutParser::createView(const LayoutOptions& options)
{
    return std::make_shared<OverlayLayout>(options.width, options.height);
}
