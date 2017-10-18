#include "ImageRender.hpp"
#include "Region.hpp"
#include "MainLayout.hpp"

#include <wx/dcclient.h>
#include <functional>

wxDEFINE_EVENT(TEST_PAINT, wxCommandEvent);

ImageRender::ImageRender(wxWindow* parent,
                         Region* region,
                         const wxString& fileName,
                         const wxPoint& pos,
                         const wxSize& size) :
    wxWindow(parent, wxID_ANY, pos, size)
{
    m_image.SetOption(wxIMAGE_OPTION_MAX_WIDTH, size.GetWidth());
    m_image.SetOption(wxIMAGE_OPTION_MAX_HEIGHT, size.GetHeight());
    m_image.LoadFile(fileName, wxBITMAP_TYPE_ANY);

    // each ImageRenderer has own flag which indicates that wxEVT_PAINT emitted (set to true)
    // when loop passes flag should be set to false
    // if flag was set to false then we should not wait for this event

    Bind(wxEVT_PAINT, [=](wxPaintEvent& event){
        OnPaint(event);
        std::cout << "post event " << pos.x << std::endl;
        wxCommandEvent repaint{TEST_PAINT};
        repaint.SetString(wxString::FromDouble(pos.x));
        wxPostEvent(this, repaint);
    });


}

void ImageRender::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    wxBitmap bt(m_image);
    dc.DrawBitmap(bt, 0, 0, false);
}
