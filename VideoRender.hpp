#ifndef VIDEORENDER_HPP
#define VIDEORENDER_HPP

#include <wx/mediactrl.h>

class VideoRender : public wxMediaCtrl
{
public:
    VideoRender(wxWindow* parent,
                const wxString& fileName,
                const wxPoint& pos,
                const wxSize& size,
                bool looped,
                bool muted);

    void OnVideoStopped(wxMediaEvent& event);

private:
    bool m_looped;

};

#endif // VIDEORENDER_HPP
