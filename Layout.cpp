#include "Layout.hpp"
#include "Image.hpp"
#include "Video.hpp"
#include "Region.hpp"
#include "WebViewWrapper.hpp"

#include <wx/frame.h>
#include <wx/mediactrl.h>
#include <wx/dcclient.h>
#include <wx/webview.h>

Layout::Layout(wxWindow* parent,
               wxWindowID id,
               bool disableMouse,
               const wxPoint& pos,
               const wxSize& size,
               long style) :
    wxFrame(parent, id, wxString{}, pos, size, style),
    m_backgroundImage("Menu/25831.jpg")
{
    if(disableMouse)
        SetCursor(wxCursor(wxCURSOR_BLANK));

    Bind(wxEVT_PAINT, &Layout::OnPaint, this);

    m_sizer = std::make_unique<wxBoxSizer>(wxVERTICAL);

//    auto region1 = new Region{this, wxPoint(420, 420),  wxSize(150, 150), 2};
//    region1->AddMedia({std::make_shared<Image>("8624.jpg"),
//                       std::make_shared<Image>("8627.jpg"),
//                       std::make_shared<Image>("8630.jpg"),
//                       std::make_shared<Image>("8633.png")});
//    auto region2 = new Region{this, wxPoint(285, 285),  wxSize(150, 150), 2};
//    region2->AddMedia({std::make_shared<Image>("8627.jpg"),
//                       std::make_shared<Image>("8630.jpg"),
//                       std::make_shared<Image>("8633.png"),
//                       std::make_shared<Image>("8624.jpg")});
//    auto region3 = new Region{this, wxPoint(150, 150),  wxSize(150, 150), 2};
//    region3->AddMedia({std::make_shared<Image>("8630.jpg"),
//                       std::make_shared<Image>("8633.png"),
//                       std::make_shared<Image>("8624.jpg"),
//                       std::make_shared<Image>("8627.jpg")});
//    auto region4 = new Region{this, wxPoint(15, 15),  wxSize(150, 150), 2};
//    region4->AddMedia({std::make_shared<Image>("8633.png"),
//                       std::make_shared<Image>("8624.jpg"),
//                       std::make_shared<Image>("8627.jpg"),
//                       std::make_shared<Image>("8630.jpg")});

//    auto region = new Region{this, wxPoint(0, 0),  wxSize(1024, 640), 2};
//    region->AddMedia({std::make_shared<Video>("28304.mp4", false, true)});

//    auto wrapper = new WebViewWrapper(this, wxID_ANY, "", wxDefaultPosition, wxSize(1366, 768));
//    wrapper->webView()->LoadURL("file:///home/stivius/xibo-examples/MenuOverlay/295.htm");

    SetSizer(m_sizer.get());

}

void Layout::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    dc.SetBrush(wxBrush(wxColour("black")));
    dc.DrawRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight());
    dc.DrawBitmap(wxBitmap(m_backgroundImage), 0, 0, false);
}
