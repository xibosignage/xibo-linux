#include "MainWindow.hpp"

MainWindow::MainWindow(wxWindow* parent,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size) :
    wxFrame(parent, id, wxString{}, pos, size)
{
    SetCursor(wxCursor(wxCURSOR_BLANK));
}
