#include "Region.hpp"

Region::Region(uint id,
               const Size& size,
               const Point& pos,
               int zindex,
               bool looped,
               const Transition& transition) :
    m_id(id),
    m_size(size),
    m_pos(pos),
    m_zindex(zindex),
    m_looped(looped),
    m_transition(transition)
{
}

uint Region::id() const
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

const Transition& Region::transition() const
{
    return m_transition;
}

void Region::show()
{
    if(!m_medias.empty())
    {
        Gtk::Fixed::show();
        m_medias.front()->show();
    }
}

void Region::on_media_timeout()
{
    if(m_medias.size() > 1)
    {
        m_previousIndex = m_currentIndex;
        m_medias[m_previousIndex]->hide();

        m_currentIndex = m_currentIndex + 1 >= m_medias.size() ? 0 : m_currentIndex + 1;
        m_medias[m_currentIndex]->show();
    }
}
