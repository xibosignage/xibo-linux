#include "VideoBackend.hpp"

double VideoBackend::GetPlaybackRate()
{
    return 0;
}

bool VideoBackend::SetPlaybackRate(double)
{
    return false;
}

double VideoBackend::GetVolume()
{
    return 0;
}

bool VideoBackend::SetVolume(double)
{
    return false;
}

bool VideoBackend::ShowPlayerControls(wxMediaCtrlPlayerControls flags)
{
    return true;
}

wxSize VideoBackend::GetVideoSize() const
{
    return wxDefaultSize;
}

void VideoBackend::Move(int WXUNUSED(x), int WXUNUSED(y), int WXUNUSED(w), int WXUNUSED(h))
{
}

bool VideoBackend::CreateControl(wxControl* ctrl,
                                 wxWindow* parent,
                                 wxWindowID id,
                                 const wxPoint& pos,
                                 const wxSize& size,
                                 long style,
                                 const wxValidator& validator,
                                 const wxString& name)
{
    if (ctrl->wxControl::Create(parent, id, pos, size, (style & ~wxBORDER_MASK) | wxBORDER_NONE, validator, name))
    {
        return false;
    }
    m_ctrl = wxStaticCast(ctrl, wxMediaCtrl);

    return true;
}

bool VideoBackend::Play()
{
    return true;
}

bool VideoBackend::Pause()
{
    return true;
}

bool VideoBackend::Stop()
{
    return true;
}

bool VideoBackend::Load(const wxString& fileName)
{
    return false;
}

bool VideoBackend::Load(const wxURI& location)
{
    return false;
}

bool VideoBackend::Load(const wxURI& WXUNUSED(location), const wxURI& WXUNUSED(proxy))
{
    return false;
}

wxMediaState VideoBackend::GetState()
{
    return wxMEDIASTATE_STOPPED;
}

bool VideoBackend::SetPosition(wxLongLong where)
{
    return true;
}

wxLongLong VideoBackend::GetPosition()
{
    return 0;
}

wxLongLong VideoBackend::GetDuration()
{
    return 0;
}
