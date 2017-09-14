#ifndef XIBOPLAYERAPP_HPP
#define XIBOPLAYERAPP_HPP

#include <thread>
#include <wx/event.h>

class MainWindow;

class PlayerProcessHandler : public wxEvtHandler
{
public:
    PlayerProcessHandler(MainWindow* parent);
    ~PlayerProcessHandler();

    void Run();
    void Stop();

private:
    void PostEvent(wxEventType type, const wxString& message);
    void CreateProcess();

private:
    MainWindow* m_parent = nullptr;
    std::thread m_processThread;
    pid_t m_processId;

};

#endif // XIBOPLAYERAPP_HPP
