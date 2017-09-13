#ifndef XIBOWATCHGODWINDOW_HPP
#define XIBOWATCHGODWINDOW_HPP

#include <wx/frame.h>

class wxTextCtrl;

class XiboWatchdogWindow : public wxFrame
{
public:
    XiboWatchdogWindow();

private:
    void OnAppOpened(wxCommandEvent& event);
    void OnLogAdded(wxCommandEvent& event);

    void InitMenuBar();
    void InitLogEdit();
    void InitButtons();

    void LogMessage(const wxString& logMessage);

private:
    wxTextCtrl* m_logEdit;
    wxSizer* m_btnSizer;
    wxSizer* m_mainSizer;

};

#endif // XIBOWATCHGODWINDOW_HPP
