#include "Video.hpp"
#include "VideoHandler.hpp"

const double MIN_VOLUME = 0.0;
const double MAX_VOLUME = 1.0;

Video::Video(int id, int duration, const std::string& uri, bool muted, bool looped) :
    Media(id, duration, Render::Native, uri), m_muted(muted), m_looped(looped)
{
    m_handler = new VideoHandler{m_uri, Size{}};
    m_handler->signal_video_ended().connect([=](){
        if(m_looped)
        {
            m_handler->play();
        }
    });

    m_handler->set_volume(m_muted ? MIN_VOLUME : MAX_VOLUME);
    m_handler->show_all();
    m_handler->play();
}

void Video::hide()
{
    m_handler->hide();
    Media::hide();
}

void Video::show()
{
    m_handler->show();
    Media::show();
}

bool Video::muted() const
{
    return m_muted;
}

bool Video::looped() const
{
    return m_looped;
}

void Video::set_size(const Size& size)
{
    m_handler->set_size(size.width, size.height);
}

Gtk::Widget& Video::handler()
{
    return *m_handler;
}
