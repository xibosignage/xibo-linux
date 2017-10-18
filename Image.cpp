#include "Image.hpp"
#include "Region.hpp"
#include "ImageRender.hpp"

Image::Image(const wxString& fileName) :
    m_fileName(fileName)
{

}

void Image::InitRender(wxWindow* parent, Region* region, const wxPoint& pos, const wxSize& size)
{
    m_imageRender = std::make_unique<ImageRender>(parent, region, m_fileName, pos, size);
}

void Image::Hide()
{
    m_imageRender->Hide();
}

void Image::Show()
{
    m_imageRender->Show();
}
