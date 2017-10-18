#ifndef RENDER_HPP
#define RENDER_HPP

#include "Media.hpp"
#include <wx/image.h>

class Region;

class ImageRender : public wxWindow
{
public:
    ImageRender(wxWindow* parent,
                Region* region,
                const wxString& fileName,
                const wxPoint& pos,
                const wxSize& size);

    void OnPaint(wxPaintEvent& event);

private:
    wxImage m_image;

};

#endif // RENDER_HPP
