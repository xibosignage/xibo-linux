#ifdef USE_GTK
#include "control/widgets/gtk/FixedContainerGtk.hpp"
#endif

std::unique_ptr<Xibo::FixedContainer> FixedContainerFactory::create(int width, int height)
{
#ifdef USE_GTK
    auto container = std::make_unique<FixedContainerGtk>();
    container->setSize(width, height);
    return container;
#else
    return nullptr;
#endif
}
