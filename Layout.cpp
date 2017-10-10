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
    // wxCURSOR_ARROW to return arrow
    if(disableMouse)
        SetCursor(wxCursor(wxCURSOR_BLANK));

    Bind(wxEVT_PAINT, &Layout::OnPaint, this);

    m_sizer = std::make_unique<wxBoxSizer>(wxVERTICAL);

    auto wrapper = new WebViewWrapper(this, wxID_ANY, "", wxDefaultPosition, wxSize(1366, 768));
    wrapper->webView()->LoadURL("file:///home/stivius/xibo-examples/MenuOverlay/295.htm");

    SetSizer(m_sizer.get());

}

void Layout::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    dc.SetBrush(wxBrush(wxColour("black")));
    dc.DrawRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight());
    dc.DrawBitmap(wxBitmap(m_backgroundImage), 0, 0, false);
}
