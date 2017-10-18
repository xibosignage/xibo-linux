#ifndef REGION_HPP
#define REGION_HPP

#include "Media.hpp"
#include "EventsQueue.hpp"

#include <wx/window.h>
#include <wx/timer.h>
#include <memory>
#include <iostream>

class MainLayout;

class Region : public wxWindow
{
public:
    Region(MainLayout* parent,
           const wxPoint& pos,
           const wxSize& size,
           int duration,
           int zindex);

    void AddMedia(const std::shared_ptr<Media>& media);
    void AddMedia(std::initializer_list<std::shared_ptr<Media>> medias);

    MainLayout* GetLayout();
    void Test(wxCommandEvent& ev);

//private:
    static int instanceCount;
    MainLayout* m_layout = nullptr;

    std::vector<std::shared_ptr<Media>> m_medias;
    wxTimer m_timer;
    int m_duration;
    int m_zindex;
    int m_creationOrder;

    size_t m_currentIndex = 0;
    size_t m_previousIndex = 0;

};

#endif // REGION_HPP
