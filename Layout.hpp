#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#include <wx/frame.h>
#include <wx/sizer.h>

class Layout : public wxFrame
{
public:
    Layout(wxWindow *parent,
           wxWindowID id,
           bool disableMouse,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxDEFAULT_FRAME_STYLE);

private:
    void OnPaint(wxPaintEvent& event);

private:
    wxBoxSizer* m_sizer;

};

#endif // MYWINDOW_HPP
