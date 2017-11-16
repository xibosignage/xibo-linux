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
    update_video_size();
//    m_handler->Bind(wxEVT_MEDIA_FINISHED, [=](wxMediaEvent& WXUNUSED(event)){
//        if(m_looped)
//        {
//            m_handler->Play();
//        }
//    });
    m_handler->set_volume(m_muted ? MIN_VOLUME : MAX_VOLUME);
    m_handler->play();
    m_handler->show_all();

    region->add(*m_handler);
}

void Video::update_video_size()
{
//    auto bestSize = m_handler->GetBestSize();
//    auto neededFactor = (bestSize.GetWidth() / static_cast<float>(bestSize.GetHeight()));
//    auto currentFactor = (m_size.GetWidth() / static_cast<float>(m_size.GetHeight()));

//    if(currentFactor > neededFactor)
//    {
//        m_handler->SetSize(std::round(m_size.GetHeight() * neededFactor), m_size.GetHeight());
//    }
//    else
//    {
//        m_handler->SetSize(m_size.GetWidth(), std::round(m_size.GetWidth() / neededFactor));
//    }
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
