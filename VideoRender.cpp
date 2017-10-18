#include "VideoRender.hpp"

const double MIN_VOLUME = 0.0;
const double MAX_VOLUME = 1.0;

#include <iostream>

VideoRender::VideoRender(wxWindow* parent,
                         const wxString& fileName,
                         const wxPoint& pos,
                         const wxSize& size) :
    wxMediaCtrl()
{
    Create(parent, wxID_ANY, fileName, pos, wxDefaultSize);

    auto bestSize = GetBestSize();
    auto neededFactor = (bestSize.GetWidth() / static_cast<float>(bestSize.GetHeight()));
    auto currentFactor = (size.GetWidth() / static_cast<float>(size.GetHeight()));

    if(currentFactor > neededFactor)
    {
        SetSize(std::round(size.GetHeight() * neededFactor), size.GetHeight());
    }
    else
    {
        SetSize(size.GetWidth(), std::round(size.GetWidth() / neededFactor));
    }

    SetWindowStyle(wxBORDER_NONE);
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
