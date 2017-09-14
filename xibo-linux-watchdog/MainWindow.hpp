#ifndef XIBOWATCHGODWINDOW_HPP
#define XIBOWATCHGODWINDOW_HPP

#include <wx/frame.h>
#include <memory>

class wxButton;
class wxTextCtrl;
class PlayerProcessHandler;

class MainWindow : public wxFrame
{
public:
    MainWindow();

    void LogMessage(const wxString& logMessage);
    void OnPlayerTerminated(wxCommandEvent& event);

private:
    void OnAppStarted(wxCommandEvent& event);
    void OnAppStopped(wxCommandEvent& event);

    void InitMenuBar();
    void InitLogEdit();
    void InitButtons();

private:
    wxTextCtrl* m_logEdit = nullptr;
    wxButton* m_startPlayerBtn = nullptr;
    wxButton* m_stopPlayerBtn = nullptr;
    wxSizer* m_btnSizer = nullptr;
    wxSizer* m_mainSizer = nullptr;

    std::unique_ptr<PlayerProcessHandler> m_playerApp;

};

#endif // XIBOWATCHGODWINDOW_HPP
