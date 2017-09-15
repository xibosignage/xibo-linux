#ifndef XIBOPLAYERAPP_HPP
#define XIBOPLAYERAPP_HPP

#include <thread>
#include <wx/event.h>

class MainWindow;

class PlayerProcessHandler : public wxEvtHandler
{
public:
    PlayerProcessHandler(MainWindow* parentWindow);
    ~PlayerProcessHandler();

    bool isRunning();
    void Run();
    void Stop(int status);

private:
    void PostProcessEvent(wxEventType type, int value);
    void CreateProcess();

private:
    MainWindow* m_parentWindow = nullptr;
    std::thread m_processThread;
    bool m_isRunning = false;
    pid_t m_processId;

};

#endif // XIBOPLAYERAPP_HPP
