#include "OverlayLayoutBuilder.hpp"
#include "ParsedLayout.hpp"

#include "control/common/OverlayLayout.hpp"

#include "common/fs/FilePath.hpp"

std::shared_ptr<IOverlayLayout> OverlayLayoutBuilder::createView(const LayoutOptions& options)
{
    return std::make_shared<OverlayLayout>(options.width, options.height);
}
