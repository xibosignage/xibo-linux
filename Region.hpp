#ifndef REGION_HPP
#define REGION_HPP

#include "Media.hpp"

#include <wx/window.h>
#include <wx/timer.h>
#include <memory>

class Region : public wxWindow
{
public:
    Region(wxWindow *parent,
           const wxPoint& pos,
           const wxSize& size,
           int duration);

    void AddMedia(const std::shared_ptr<Media>& media);
    void AddMedia(std::initializer_list<std::shared_ptr<Media>> medias);

private:
    std::vector<std::shared_ptr<Media>> m_medias;
    wxTimer m_timer;
    int m_duration;

    size_t m_currentIndex = 0;
    size_t m_previousIndex = 0;

};

#endif // REGION_HPP
