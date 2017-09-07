#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#include <wx/frame.h>

class MainWindow : public wxFrame
{
public:
    MainWindow(wxWindow *parent,
               wxWindowID id,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize);
};

#endif // MYWINDOW_HPP
