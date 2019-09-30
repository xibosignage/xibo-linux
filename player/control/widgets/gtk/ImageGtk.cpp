#include "control/widgets/gtk/ImageGtk.hpp"

#include "common/fs/FilePath.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/types/Uri.hpp"

const int BitsPerSample = 8;

ImageGtk::ImageGtk(int width, int height, bool useAlpha) : WidgetGtk(handler_)
{
    checkSize(width, height);
    handler_.set(Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, useAlpha, BitsPerSample, width, height));
}

int ImageGtk::width() const
{
    assert(pixbuf());
    return pixbuf()->get_width();
}

int ImageGtk::height() const
{
    assert(pixbuf());
    return pixbuf()->get_height();
}

void ImageGtk::setSize(int width, int height)
{
    checkSize(width, height);
    auto newPixbuf = pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
    handler_.set(newPixbuf);
}

void ImageGtk::setColor(const Color& color)
{
    assert(pixbuf());
    pixbuf()->fill(color.hex());
}

void ImageGtk::loadFrom(const Uri& uri, PreserveRatio preserveRatio)
{
    checkUri(uri);
    handler_.set(Gdk::Pixbuf::create_from_file(uri.path(), width(), height(), static_cast<bool>(preserveRatio)));
}

Gtk::Image& ImageGtk::get()
{
    return handler_;
}

Glib::RefPtr<const Gdk::Pixbuf> ImageGtk::pixbuf() const
{
    return handler_.get_pixbuf();
}

Glib::RefPtr<Gdk::Pixbuf> ImageGtk::pixbuf()
{
    return handler_.get_pixbuf();
}

void ImageGtk::checkSize(int width, int height)
{
    if (width < 0 || height < 0) throw ImageGtk::Error{"size should be positive"};
}

void ImageGtk::checkUri(const Uri& uri)
{
    if (!FileSystem::exists(uri.path())) throw ImageGtk::Error{"image " + uri.path() + " was not found"};
}
