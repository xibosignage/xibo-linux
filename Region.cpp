#include "Region.hpp"

#include <iostream>

Region::Region(wxWindow* parent,
               const wxPoint& pos,
               const wxSize& size,
               int duration) :
    wxWindow(parent, wxID_ANY, pos, size),
    m_duration(duration)
{
    m_timer.Start(duration * 1000);

    m_timer.Bind(wxEVT_TIMER, [=](wxTimerEvent& WXUNUSED(event)){
        if(m_medias.size() > 1)
        {
            m_previousIndex = m_currentIndex;
            m_medias[m_previousIndex]->Hide();

            m_currentIndex = m_currentIndex + 1 >= m_medias.size() ? 0 : m_currentIndex + 1;
            m_medias[m_currentIndex]->Show();
        }
    });
}

void Region::AddMedia(const std::shared_ptr<Media>& media)
{
    assert(media);

    media->InitRender(GetParent(), GetPosition(),  GetSize());
    m_medias.push_back(media);
}

void Region::AddMedia(std::initializer_list<std::shared_ptr<Media>> medias)
{
    assert(medias.size() > 0);

    for(auto&& media : medias)
    {
        AddMedia(media);
        media->Hide();
    }

    m_medias[m_currentIndex]->Show();
}
