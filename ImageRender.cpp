#include "ImageRender.hpp"

#include <wx/image.h>
#include <wx/dcclient.h>

ImageRender::ImageRender(wxWindow* parent,
                         const wxString& fileName,
                         const wxPoint& pos,
                         const wxSize& size) :
    wxWindow(parent, wxID_ANY, pos, size)
{
    m_image.SetOption(wxIMAGE_OPTION_MAX_WIDTH, size.GetWidth());
    m_image.SetOption(wxIMAGE_OPTION_MAX_HEIGHT, size.GetHeight());
    m_image.LoadFile(fileName, wxBITMAP_TYPE_ANY);

    Bind(wxEVT_PAINT, &ImageRender::OnPaint, this);
}


void ImageRender::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    wxBitmap bt(m_image);
    dc.DrawBitmap(bt, 0, 0, false);
}
