#include "Layout.hpp"
#include "ImageRender.hpp"

#include <wx/frame.h>
#include <wx/mediactrl.h>

#include <iostream>

Layout::Layout(wxWindow* parent,
               wxWindowID id,
               bool disableMouse,
               const wxPoint& pos,
               const wxSize& size,
               long style) :
    wxFrame(parent, id, wxString{}, pos, size, style)
{
    // wxCURSOR_ARROW to return arrow
    if(disableMouse)
        SetCursor(wxCursor(wxCURSOR_BLANK));        

    auto back = new ImageRender{this, "8621.jpg", wxDefaultPosition,  wxSize(1366, 768)};
    auto img4 = new ImageRender{this, "8633.png", wxPoint(420, 420),  wxSize(150, 150)};
    auto img3 = new ImageRender{this, "8630.jpg", wxPoint(285, 285),  wxSize(150, 150)};
    auto img1 = new ImageRender{this, "8627.jpg", wxPoint(150, 150),  wxSize(150, 150)};
    auto img2 = new ImageRender{this, "8624.jpg", wxPoint(15, 15),  wxSize(150, 150)};

    //auto m = new wxMediaCtrl(this, wxID_ANY, "00007.MTS", wxPoint(700, 0), wxSize(600, 600));
    //m->Play();

}
