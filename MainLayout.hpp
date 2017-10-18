#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#include <wx/frame.h>
#include <wx/image.h>
#include <wx/sizer.h>

#include <memory>

wxDECLARE_EVENT(TEST_PAINT, wxCommandEvent);

class MainLayout : public wxFrame
{
public:
    MainLayout(wxWindow *parent,
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

    void Test(wxCommandEvent& ev);
};

#endif // MYWINDOW_HPP
