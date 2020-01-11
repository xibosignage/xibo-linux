#ifdef USE_GTK
#include "control/widgets/gtk/OverlayContainerGtk.hpp"
#endif
#include "control/widgets/FixedContainer.hpp"

std::unique_ptr<Xibo::OverlayContainer> OverlayContainerFactory::create(int width, int height)
{
#ifdef USE_GTK
    auto container = std::make_unique<OverlayContainerGtk>();
    container->setSize(width, height);
    container->setOverlayContainer(FixedContainerFactory::create(width, height));
    return container;
#else
    return nullptr;
#endif
}
