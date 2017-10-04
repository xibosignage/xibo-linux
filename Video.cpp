#include "Video.hpp"

#include <wx/event.h>

Video::Video(const wxString& fileName, bool looped, bool muted) :
    m_fileName(fileName),
    m_looped(looped),
    m_muted(muted)
{

}

void Video::InitRender(wxWindow* parent, const wxPoint& pos, const wxSize& size)
{
    m_videoRender = std::make_unique<VideoRender>(parent, m_fileName, pos, size);
    m_videoRender->InitVolume(m_muted);

//    m_videoRender->Bind(wxEVT_MEDIA_FINISHED, [=](wxMediaEvent& WXUNUSED(event)){
//        if(m_looped)
//        {
//            m_videoRender->Play();
//        }
//    });

//    m_videoRender->Play();
}

void Video::Hide()
{
    m_videoRender->Hide();
}

void Video::Show()
{
    m_videoRender->Show();
}
