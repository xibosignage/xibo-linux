#include "creators/InstanceFactory.hpp"

#include "adaptors/GtkOverlayAdaptor.hpp"
#include "adaptors/GtkImageAdaptor.hpp"
#include "adaptors/GtkFixedLayoutAdaptor.hpp"

#include "control/MainLayout.hpp"
#include "control/MediaContainer.hpp"
#include "control/Background.hpp"

using MainLayoutFactory = InstanceFactory<MainLayout, GtkOverlayAdaptor>;
using MediaContainerFactory = InstanceFactory<MediaContainer, GtkFixedLayoutAdaptor>;
using BackgroundFactory = InstanceFactory<Background, GtkImageAdaptor>;
