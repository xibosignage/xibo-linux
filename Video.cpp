#include "Video.hpp"
#include "Region.hpp"
#include "VideoHandler.hpp"

const double MIN_VOLUME = 0.0;
const double MAX_VOLUME = 1.0;

Video::Video(const std::string& filename, bool looped, bool muted) :
    m_filename(filename),
    m_looped(looped),
    m_muted(muted)
{

}

void Video::init(MyRegion* region, const Point& pos, const Size& size, int zindex)
{
    Media::init(region, pos, size, zindex);

    m_handler = new VideoHandler{m_filename, m_size};

    m_handler->signal_video_ended().connect([=](){
        if(m_looped)
        {
            m_handler->play();
        }
    });

    m_handler->set_volume(m_muted ? MIN_VOLUME : MAX_VOLUME);
    m_handler->show_all();
    m_handler->play();

    region->add(*m_handler);
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
