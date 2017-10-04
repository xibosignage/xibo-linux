#ifndef VIDEOBACKEND_HPP
#define VIDEOBACKEND_HPP

#include <wx/mediactrl.h>

class WXDLLIMPEXP_MEDIA VideoBackend : public wxMediaBackendCommonBase
{
public:
    VideoBackend() = default;
    virtual ~VideoBackend() { }
    virtual bool CreateControl(wxControl* ctrl, wxWindow* parent,
                                    wxWindowID id,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long style,
                                    const wxValidator& validator,
                                    const wxString& name);
    virtual bool Play();
    virtual bool Pause();
    virtual bool Stop();
    virtual bool Load(const wxString& fileName);
    virtual bool Load(const wxURI& location);
    virtual bool Load(const wxURI& location, const wxURI& proxy);
    virtual wxMediaState GetState();
    virtual bool SetPosition(wxLongLong where);
    virtual wxLongLong GetPosition();
    virtual wxLongLong GetDuration();
    virtual void Move(int x, int y, int w, int h);
    wxSize GetVideoSize() const;
    virtual double GetPlaybackRate();
    virtual bool SetPlaybackRate(double);
    virtual double GetVolume();
    virtual bool SetVolume(double);
    virtual bool ShowPlayerControls(wxMediaCtrlPlayerControls flags);

    wxDECLARE_DYNAMIC_CLASS(VideoBackend);
};

wxIMPLEMENT_DYNAMIC_CLASS(VideoBackend, wxMediaBackend);

#endif // VIDEOBACKEND_HPP
