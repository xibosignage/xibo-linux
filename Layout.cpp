#include "Layout.hpp"
#include "Image.hpp"
#include "Video.hpp"
#include "Region.hpp"

#include <wx/frame.h>
#include <wx/mediactrl.h>
#include <wx/dcclient.h>

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

    SetSizer(m_sizer.get());

}

void Layout::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    dc.SetBrush(wxBrush(wxColour("black")));
    dc.DrawRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight());
    dc.DrawBitmap(wxBitmap(m_backgroundImage), 0, 0, false);
}
