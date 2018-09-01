#include "Region.hpp"
#include "media/IMedia.hpp"

#include "GtkFixedLayoutWrapper.hpp"

Region::Region(int id, int width, int height, int left, int top, int zindex, bool looped) :
    m_id(id), m_left(left), m_top(top), m_zindex(zindex), m_looped(looped)
{
    m_handler = std::make_shared<GtkFixedLayoutWrapper>();
    set_size(width, height);
}

Region::~Region()
{
}

int Region::width() const
{
    return m_handler->width();
}

int Region::height() const
{
    return m_handler->height();
}

void Region::set_size(int width, int height)
{
    m_handler->set_size(width, height);
    for(auto&& media : m_media)
    {
        media->set_size(width, height);
    }
}

int Region::left() const
{
    return m_left;
}

int Region::top() const
{
    return m_top;
}

void Region::set_pos(int left, int top)
{
    m_left = left;
    m_top = top;
}

int Region::id() const
{
    return m_id;
}

int Region::zindex() const
{
    return m_zindex;
}

bool Region::looped() const
{
    return m_looped;
}

void Region::show()
{
    if(!m_media.empty())
    {
        for(auto&& media : m_media)
            media->request_handler();

        m_handler->show();
        m_media[m_currentMediaIndex]->start();
    }
}

void Region::add_media(std::unique_ptr<IMedia> media)
{
    media->handler_requested().connect([=](Gtk::Widget& widget, int left, int top){
         m_handler->add_child(widget, left, top); // FIXME move after reallocation
    });
    media->media_timeout().connect(sigc::mem_fun(*this, &Region::on_media_timeout));
    m_media.push_back(std::move(media));
}

IFixedLayoutWrapper& Region::handler()
{
    return *m_handler;
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
