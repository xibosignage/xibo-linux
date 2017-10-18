#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Media.hpp"
#include "ImageRender.hpp"

#include <memory>

class Image : public Media
{
public:
    Image(const wxString& fileName);

    void InitRender(wxWindow* parent, Region* region, const wxPoint& pos, const wxSize& size) override;
    void Hide() override;
    void Show() override;

//private:
    wxString m_fileName;    
    std::unique_ptr<ImageRender> m_imageRender;

};

#endif // IMAGE_HPP
