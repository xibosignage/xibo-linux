#include "Image.hpp"

Image::Image(const wxString& fileName) :
    m_fileName(fileName)
{

}

void Image::InitRender(wxWindow* parent, const wxPoint& pos, const wxSize& size)
{
    m_imageRender = std::make_unique<ImageRender>(parent, m_fileName, pos, size);
}

void Image::Hide()
{
    m_imageRender->Hide();
}

void Image::Show()
{
    m_imageRender->Show();
}
