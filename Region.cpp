#include "Region.hpp"

#include <cassert>
#include <glibmm.h>

Region::Region(int id,
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
//    Glib::signal_timeout().connect([=](){
//        if(m_medias.size() > 1)
//        {
//            m_previousIndex = m_currentIndex;
//            m_medias[m_previousIndex]->hide();

//            m_currentIndex = m_currentIndex + 1 >= m_medias.size() ? 0 : m_currentIndex + 1;
//            m_medias[m_currentIndex]->show();
//        }
//        return true;
//    }, m_duration * 1000);
}

// fix it
// receive parsed object, remove set_size
void Region::add_media(const std::shared_ptr<Media>& media)
{
    assert(media);

    media->set_size(m_size);
    put(media->handler(), 0, 0);
    m_medias.push_back(media);
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

const Transition& Region::transition() const
{
    return m_transition;
}
