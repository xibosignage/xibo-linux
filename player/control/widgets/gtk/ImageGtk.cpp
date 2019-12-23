#include "control/widgets/gtk/ImageGtk.hpp"

#include "common/fs/FilePath.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/types/Uri.hpp"

const int BitsPerSample = 8;

ImageGtk::ImageGtk(int width, int height, bool useAlpha) : WidgetGtk(handler_)
{
    check(width, height);
    set(Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, useAlpha, BitsPerSample, width, height));
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
    check(width, height);
    set(pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR));
}

void ImageGtk::setColor(const Color& color)
{
    assert(pixbuf());
    pixbuf()->fill(color.hex());
}

void ImageGtk::loadFrom(const Uri& uri, PreserveRatio preserveRatio)
{
    try
    {
        set(Gdk::Pixbuf::create_from_file(uri.path(), width(), height(), static_cast<bool>(preserveRatio)));
    }
    catch (Glib::Error& e)
    {
        throw Error{"ImageGtk", static_cast<std::string>(e.what())};
    }
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

void ImageGtk::check(int width, int height)
{
    if (width <= 0 || height <= 0) throw Error{"ImageGtk", "Size should be positive"};
}

void ImageGtk::set(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf)
{
    if (!pixbuf) throw Error{"ImageGtk", "Not enough memory to allocate image"};
    handler_.set(pixbuf);
}
