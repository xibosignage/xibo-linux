#ifndef XIBOWATCHGODWINDOW_HPP
#define XIBOWATCHGODWINDOW_HPP

#include <wx/frame.h>
#include <wx/timer.h>

#include <memory>
#include <signal.h>

class wxButton;
class wxTextCtrl;
class PlayerProcessHandler;
class SystemTrayIcon;

class MainWindow : public wxFrame
{
public:
    MainWindow();

    void OnPlayerStarted(wxCommandEvent& event);
    void OnPlayerStartedError(wxCommandEvent& event);
    void OnPlayerClosed(wxCommandEvent& event);

private:
    void OnBtnPlayerStart(wxCommandEvent& event);
    void OnBtnPlayerTerminated(wxCommandEvent& event);
    void OnBtnPlayerKilled(wxCommandEvent& event);
    void OnRestartTimerFinished(wxTimerEvent& event);

    void LogMessage(const wxString& logMessage);

    void InitMenuBar();
    void InitLogEdit();
    void InitButtons();
    void UpdateButtons();

private:
    wxTextCtrl* m_logEdit = nullptr;
    wxButton* m_startPlayerBtn = nullptr;
    wxButton* m_terminatePlayerBtn = nullptr;
    wxButton* m_killPlayerBtn = nullptr;
    wxSizer* m_btnSizer = nullptr;
    wxSizer* m_mainSizer = nullptr;
    wxMenu* m_menu = nullptr;
    wxMenuBar* m_menuBar = nullptr;

    wxTimer m_restartTimer;

    std::shared_ptr<PlayerProcessHandler> m_playerApp;
    std::unique_ptr<SystemTrayIcon> m_systemTrayIcon;

};

#endif // XIBOWATCHGODWINDOW_HPP
