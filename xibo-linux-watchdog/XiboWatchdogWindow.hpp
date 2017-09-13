#ifndef XIBOWATCHGODWINDOW_HPP
#define XIBOWATCHGODWINDOW_HPP

#include <wx/frame.h>
#include <thread>

class wxTextCtrl;

class XiboWatchdogWindow : public wxFrame
{
public:
    XiboWatchdogWindow();
    ~XiboWatchdogWindow();

private:
    void OnAppOpened(wxCommandEvent& event);
    void OnLogAdded(wxCommandEvent& event);

    void InitMenuBar();
    void InitLogEdit();
    void InitButtons();

    void LogMessage(const wxString& logMessage);
    void CreateProcess();

private:
    wxTextCtrl* m_logEdit;
    wxSizer* m_btnSizer;
    wxSizer* m_mainSizer;

    std::thread m_processThread;
    pid_t m_processId;

};

#endif // XIBOWATCHGODWINDOW_HPP
