#include "VideoRender.hpp"

const double MIN_VOLUME = 0.0;
const double MAX_VOLUME = 1.0;


VideoRender::VideoRender(wxWindow* parent,
                         const wxString&,
                         const wxPoint& pos,
                         const wxSize& size) :
    wxMediaCtrl(parent, wxID_ANY, "00007.MTS", pos, size, wxBORDER_NONE)
{

}

void VideoRender::InitVolume(bool muted)
{
    if(muted)
    {
        SetVolume(MIN_VOLUME);
    }
    else
    {
        SetVolume(MAX_VOLUME);
    }
}
