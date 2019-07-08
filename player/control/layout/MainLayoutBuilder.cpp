#include "MainLayoutBuilder.hpp"
#include "ParsedLayout.hpp"

#include "control/common/Image.hpp"
#include "control/common/OverlayLayout.hpp"

#include "common/FilePath.hpp"

std::shared_ptr<IOverlayLayout> MainLayoutBuilder::createView(const LayoutOptions& options)
{
    auto layoutView = std::make_shared<OverlayLayout>(options.width, options.height);

    layoutView->setMainChild(createBackground(options));

    return layoutView;
}

std::shared_ptr<IImage> MainLayoutBuilder::createBackground(const LayoutOptions& options)
{
    auto background = std::make_shared<Image>(options.width, options.height);

    if(options.backgroundUri.isValid())
        background->loadFromFile(options.backgroundUri.path(), false);
    else
        background->setColor(options.backgroundColor);

    return background;
}
