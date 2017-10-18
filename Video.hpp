#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "Media.hpp"
#include "VideoRender.hpp"

#include <memory>

class Video : public Media
{
public:
    Video(const wxString& fileName,
          bool looped,
          bool muted);

    void InitRender(wxWindow* parent, Region* region, const wxPoint& pos, const wxSize& size) override;
    void Hide() override;
    void Show() override;

private:
    wxString m_fileName;
    bool m_looped;
    bool m_muted;

    std::unique_ptr<VideoRender> m_videoRender;

};

#endif // VIDEO_HPP
