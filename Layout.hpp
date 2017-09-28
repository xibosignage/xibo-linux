#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#include <wx/frame.h>
#include <wx/image.h>
#include <wx/sizer.h>

#include <memory>

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
    std::unique_ptr<wxBoxSizer> m_sizer;
    wxImage m_backgroundImage;

};

#endif // MYWINDOW_HPP
