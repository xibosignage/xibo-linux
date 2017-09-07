#include "MainWindow.hpp"

#include "wx/frame.h"

MainWindow::MainWindow(wxWindow* parent,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size) :
    // wxSIMPLE_BORDER (removing window decorations) should be called during window initialization
    wxFrame(parent, id, wxString{}, pos, size, wxSIMPLE_BORDER)
{
    // wxCURSOR_ARROW to return arrow
    SetCursor(wxCursor(wxCURSOR_BLANK));
}
