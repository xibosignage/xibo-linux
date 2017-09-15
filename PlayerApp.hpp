#ifndef MYAPP_HPP
#define MYAPP_HPP

#include <wx/app.h>
#include "MainWindow.hpp"

const int INVALID_POS = -1;
const int INVALID_SIZE = -1;

class PlayerApp : public wxApp
{
public:
    PlayerApp();

    void OnInitCmdLine(wxCmdLineParser& parser) override;
    bool OnCmdLineParsed(wxCmdLineParser& parser) override;
    bool OnInit() override;
    int OnExit() override;

private:
    void ShowMainWindow();

private:
    MainWindow* m_mainWindow = nullptr;
    bool m_fullscreen = false;
    bool m_stayOnTop = false;
    bool m_disableMouse = false;
    long m_width = INVALID_SIZE;
    long m_height = INVALID_SIZE;
    long m_xpos = INVALID_POS;
    long m_ypos = INVALID_POS;

};

#endif // MYAPP_HPP
