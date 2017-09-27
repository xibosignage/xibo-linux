#ifndef RENDER_HPP
#define RENDER_HPP

#include <wx/window.h>
#include <wx/image.h>

class ImageRender : public wxWindow
{
public:
    ImageRender(wxWindow *parent,
                const wxString& fileName,
                const wxPoint& pos,
                const wxSize& size);

    void OnPaint(wxPaintEvent& event);

private:
    wxImage m_image;

};

#endif // RENDER_HPP
