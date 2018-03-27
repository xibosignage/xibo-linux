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
    m_medias.push_back(std::move(media));
    put(m_medias.back()->handler(), 0, 0);
}

void Region::show()
{
    if(!m_medias.empty())
    {
        Gtk::Fixed::show();
        m_medias[m_currentIndex]->show();

        startTimer(m_currentIndex);
    }
}

void Region::startTimer(size_t mediaIndex)
{
    uint duration = static_cast<uint>(m_medias[mediaIndex]->duration()) * MSECS;
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &Region::on_media_timeout), duration);
}

bool Region::on_media_timeout()
{
    if(m_medias.size() > 1)
    {
        m_previousIndex = m_currentIndex;
        m_medias[m_previousIndex]->hide();

        m_currentIndex = m_currentIndex + 1 >= m_medias.size() ? 0 : m_currentIndex + 1;
        m_medias[m_currentIndex]->show();

        //startTimer(m_currentIndex);
        return false;
    }
    return true;
}
