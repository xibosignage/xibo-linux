 #include "Region.hpp"

#include <glibmm/main.h>

const uint MSECS = 1000;

Region::Region(int id,
               const Size& size,
               const Point& pos,
               int zindex,
               bool looped) :
    m_id(id),
    m_size(size),
    m_pos(pos),
    m_zindex(zindex),
    m_looped(looped)
{
}

int Region::id() const
{
    return m_id;
}

const Size& Region::size() const
{
    return m_size;
}

const Point& Region::position() const
{
    return m_pos;
}

int Region::zindex() const
{
    return m_zindex;
}

bool Region::looped() const
{
    return m_looped;
}

void Region::add_media(std::unique_ptr<Media> media)
{
    media->handler_requested().connect([=](Gtk::Widget& widget, int x, int y){
        put(widget, x, y);
    });
    m_media.push_back(std::move(media));
}

sigc::signal<void> Region::request_handler() const
{
    return m_request_handler;
}

void Region::show()
{
    if(!m_media.empty())
    {        
        m_request_handler.emit();

        Gtk::Fixed::show();
        m_media[m_currentIndex]->start();

        start_timer(m_currentIndex);
    }
}

void Region::start_timer(size_t media_index)
{
    uint duration = static_cast<uint>(m_media[media_index]->duration()) * MSECS;
    Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &Region::on_media_timeout), duration);
}

void Region::on_media_timeout()
{
    if(m_media.size() > 1)
    {
        m_previousIndex = m_currentIndex;
        m_media[m_previousIndex]->stop();

        m_currentIndex = m_currentIndex + 1 >= m_media.size() ? 0 : m_currentIndex + 1;
        m_media[m_currentIndex]->start();

        start_timer(m_currentIndex);
    }
}
