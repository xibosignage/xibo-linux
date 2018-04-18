 #include "Region.hpp"

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
    media->handler_requested().connect([=](Gtk::Widget& widget, Point point){
        put(widget, point.left, point.top);
    });
    media->media_timeout().connect(sigc::mem_fun(*this, &Region::on_media_timeout));
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
        m_media[m_currentMediaIndex]->start();
    }
}

void Region::on_media_timeout()
{
    if(m_media.size() > 1)
    {
        m_media[m_currentMediaIndex]->stop();
        m_currentMediaIndex = m_currentMediaIndex + 1 >= m_media.size() ? 0 : m_currentMediaIndex + 1;
        m_media[m_currentMediaIndex]->start();
    }
    else
    {
        if(m_looped)
        {
            m_media[m_currentMediaIndex]->stop();
            m_media[m_currentMediaIndex]->start();
        }
    }
}
