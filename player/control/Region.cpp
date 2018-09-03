#include "Region.hpp"

#include "media/IMedia.hpp"
#include "adaptors/GtkFixedLayoutAdaptor.hpp"

Region::Region(int id, int width, int height, int left, int top, int zindex, bool looped) :
    m_id(id), m_left(left), m_top(top), m_zindex(zindex), m_looped(looped)
{
    m_handler = std::make_unique<GtkFixedLayoutAdaptor>();
    setSize(width, height);
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

void Region::setSize(int width, int height)
{
    m_handler->setSize(width, height);
    for(auto&& media : m_media)
    {
        media->setSize(width, height);
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

void Region::setPos(int left, int top)
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
            media->requestHandler();

        m_handler->show();
        m_media[m_currentMediaIndex]->start();
    }
}

void Region::addMedia(std::unique_ptr<IMedia> media)
{
    media->handlerRequested().connect([=](Gtk::Widget& widget, int left, int top){
         m_handler->addChild(widget, left, top); // FIXME move after reallocation
    });
    media->mediaTimeout().connect(sigc::mem_fun(*this, &Region::onMediaTimeout));
    m_media.push_back(std::move(media));
}

IFixedLayoutAdaptor& Region::handler()
{
    return *m_handler;
}

void Region::onMediaTimeout()
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
