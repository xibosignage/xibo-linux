#include "MainWindow.hpp"

#include "wx/frame.h"

MainWindow::MainWindow(wxWindow* parent,
                       wxWindowID id,
                       bool disableMouse,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style) :
    // wxSIMPLE_BORDER (removing window decorations) should be called during window initialization
    wxFrame(parent, id, wxString{}, pos, size, style)
{
    // wxCURSOR_ARROW to return arrow
    if(disableMouse)
        SetCursor(wxCursor(wxCURSOR_BLANK));
}
