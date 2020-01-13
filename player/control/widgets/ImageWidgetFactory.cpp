#ifdef USE_GTK
#include "control/widgets/gtk/ImageGtk.hpp"
#endif
#include "control/widgets/Image.hpp"

std::unique_ptr<Xibo::Image> ImageWidgetFactory::create(const Uri& uri,
                                                        int width,
                                                        int height,
                                                        Xibo::Image::PreserveRatio preserveRatio)
{
#ifdef USE_GTK
    auto image = std::make_unique<ImageGtk>();
    image->setSize(width, height);
    image->loadFrom(uri, preserveRatio);
    return image;
#else
    return nullptr;
#endif
}

std::unique_ptr<Xibo::Image> ImageWidgetFactory::create(const Color& color, int width, int height)
{
#ifdef USE_GTK
    auto image = std::make_unique<ImageGtk>();
    image->setSize(width, height);
    image->fillColor(color);
    return image;
#else
    return nullptr;
#endif
}
