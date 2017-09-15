#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#include <wx/frame.h>

class MainWindow : public wxFrame
{
public:
    MainWindow(wxWindow *parent,
               wxWindowID id,
               bool disableMouse,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxDEFAULT_FRAME_STYLE);

};

#endif // MYWINDOW_HPP
