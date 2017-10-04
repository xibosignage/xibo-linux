#ifndef VIDEORENDER_HPP
#define VIDEORENDER_HPP

#include <wx/mediactrl.h>

class VideoRender : public wxMediaCtrl
{
public:
    VideoRender(wxWindow* parent,
                const wxString& fileName,
                const wxPoint& pos,
                const wxSize& size);

    void InitVolume(bool muted);

private:
    void OnErase(wxEraseEvent& event);

};

#endif // VIDEORENDER_HPP
