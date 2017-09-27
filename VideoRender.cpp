#include "VideoRender.hpp"

const double MIN_VOLUME = 0.0;

VideoRender::VideoRender(wxWindow* parent,
                         const wxString& fileName,
                         const wxPoint& pos,
                         const wxSize& size,
                         bool looped,
                         bool muted) :
    wxMediaCtrl(parent, wxID_ANY, fileName, pos, size),
    m_looped(looped)
{
    Bind(wxEVT_MEDIA_FINISHED, &VideoRender::OnVideoStopped, this);

    if(muted)
    {
        SetVolume(MIN_VOLUME);
    }
}

void VideoRender::OnVideoStopped(wxMediaEvent& WXUNUSED(event))
{
    if(m_looped)
    {
        Play();
    }
}

