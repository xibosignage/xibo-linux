#include "Region.hpp"
#include "MainLayout.hpp"

#include <iostream>
#include <cassert>
#include <glibmm.h>

MyRegion::MyRegion(MainLayout* layout,
               const Point& pos,
               const Size& size,
               int duration,
               int zindex) :
    m_pos(pos),
    m_size(size),
    m_duration(duration),
    m_zindex(zindex)
{
    layout->add_to_container(this, pos);

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

void MyRegion::add_media(const std::shared_ptr<Media>& media)
{
    assert(media);

    media->init(this, m_pos, m_size, m_zindex);
    m_medias.push_back(media);
}

void MyRegion::add_media(std::initializer_list<std::shared_ptr<Media>> medias)
{
    assert(medias.size() > 0);

    for(auto&& media : medias)
    {
        add_media(media);
        media->hide();
    }

    m_medias[m_currentIndex]->show();

}

Point MyRegion::position() const
{
    return m_pos;
}

Size MyRegion::size() const
{
    return m_size;
}
