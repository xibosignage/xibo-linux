#include "Layout.hpp"
#include "Image.hpp"
#include "Video.hpp"
#include "Region.hpp"

#include <wx/frame.h>
#include <wx/mediactrl.h>
#include <wx/dcclient.h>

#include <iostream>

Layout::Layout(wxWindow* parent,
               wxWindowID id,
               bool disableMouse,
               const wxPoint& pos,
               const wxSize& size,
               long style) :
    wxFrame(parent, id, wxString{}, pos, size, style),
    m_backgroundImage("8621.jpg")
{
    // wxCURSOR_ARROW to return arrow
    if(disableMouse)
        SetCursor(wxCursor(wxCURSOR_BLANK));

    Bind(wxEVT_PAINT, &Layout::OnPaint, this);

    m_sizer = std::make_unique<wxBoxSizer>(wxVERTICAL);

    auto region1 = new Region{this, wxPoint(420, 420),  wxSize(150, 150), 2};
    region1->AddMedia({std::make_shared<Image>("8624.jpg"),
                       std::make_shared<Image>("8627.jpg"),
                       std::make_shared<Image>("8630.jpg"),
                       std::make_shared<Image>("8633.png")});
    auto region2 = new Region{this, wxPoint(285, 285),  wxSize(150, 150), 2};
    region2->AddMedia({std::make_shared<Image>("8627.jpg"),
                       std::make_shared<Image>("8630.jpg"),
                       std::make_shared<Image>("8633.png"),
                       std::make_shared<Image>("8624.jpg")});
    auto region3 = new Region{this, wxPoint(150, 150),  wxSize(150, 150), 2};
    region3->AddMedia({std::make_shared<Image>("8630.jpg"),
                       std::make_shared<Image>("8633.png"),
                       std::make_shared<Image>("8624.jpg"),
                       std::make_shared<Image>("8627.jpg")});
    auto region4 = new Region{this, wxPoint(15, 15),  wxSize(150, 150), 2};
    region4->AddMedia({std::make_shared<Image>("8633.png"),
                       std::make_shared<Image>("8624.jpg"),
                       std::make_shared<Image>("8627.jpg"),
                       std::make_shared<Image>("8630.jpg")});

    auto region5 = new Region{this, wxPoint(700, 300), wxSize(450, 450), 2};
    region5->AddMedia(std::make_shared<Video>("00007.MTS", false, false));

    SetSizer(m_sizer.get());

}

void Layout::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    dc.SetBrush(wxBrush(wxColour("black")));
    dc.DrawRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight());
    dc.DrawBitmap(wxBitmap(m_backgroundImage), 0, 0, false);
}
