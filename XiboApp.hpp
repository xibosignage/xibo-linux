#ifndef MYAPP_HPP
#define MYAPP_HPP

#include <wx/app.h>
#include <memory>

const int INVALID_POS = -1;
const int INVALID_SIZE = -1;

class MainLayout;
class EventsQueue;

class XiboApp : public wxApp
{
public:
    XiboApp();

    void OnInitCmdLine(wxCmdLineParser& parser) override;
    bool OnCmdLineParsed(wxCmdLineParser& parser) override;
    bool OnInit() override;
    int OnExit() override;

    EventsQueue& GetEventsQueue();

private:
    void ShowMainWindow();

private:
    MainLayout* m_mainWindow = nullptr;
    std::unique_ptr<EventsQueue> m_eventsQueue;

    bool m_fullscreen = false;
    bool m_stayOnTop = false;
    bool m_disableMouse = false;
    long m_width = INVALID_SIZE;
    long m_height = INVALID_SIZE;
    long m_xpos = INVALID_POS;
    long m_ypos = INVALID_POS;

};

wxDECLARE_APP(XiboApp);

#endif // MYAPP_HPP
