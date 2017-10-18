#include "XiboApp.hpp"
#include "MainLayout.hpp"
#include "EventsQueue.hpp"

#include <wx/cmdline.h>
#include <iostream>

const int DEFAULT_XPOS = 0;
const int DEFAULT_YPOS = 0;

XiboApp::XiboApp()
{
    m_eventsQueue = std::make_unique<EventsQueue>();
}

void XiboApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.AddSwitch("f", "fullscreen", "run app in fullscreen");
    parser.AddSwitch("t", "stay-on-top", "Put the window over other windows");
    parser.AddSwitch("m", "disable-mouse", "disable mouse pointer");
    parser.AddLongOption("width", "set width to the window app", wxCMD_LINE_VAL_NUMBER);
    parser.AddLongOption("height", "set height to the window app", wxCMD_LINE_VAL_NUMBER);
    parser.AddOption("x", wxString{}, "set x position to the window app", wxCMD_LINE_VAL_NUMBER);
    parser.AddOption("y", wxString{}, "set y position to the window app", wxCMD_LINE_VAL_NUMBER);

}

bool XiboApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    m_fullscreen = parser.Found("f");
    m_stayOnTop = parser.Found("t");
    m_disableMouse = parser.Found("m");

    if(!parser.Found("width", &m_width))
        m_width = INVALID_SIZE;
    if(!parser.Found("height", &m_height))
        m_height = INVALID_SIZE;
    if(!parser.Found("x", &m_xpos))
        m_xpos = INVALID_POS;
    if(!parser.Found("y", &m_ypos))
        m_ypos = INVALID_POS;

    ShowMainWindow();
   
    return true;
}


bool XiboApp::OnInit()
{
    wxInitAllImageHandlers();
    return wxApp::OnInit();
}

int XiboApp::OnExit()
{
    return wxApp::OnExit();
}

EventsQueue& XiboApp::GetEventsQueue()
{
    return *m_eventsQueue.get();
}

void XiboApp::ShowMainWindow()
{
    int width = (m_width == INVALID_SIZE) ? wxDefaultSize.GetWidth() : m_width;
    int height = (m_height == INVALID_SIZE) ? wxDefaultSize.GetHeight() : m_height;
    int xpos = (m_xpos == INVALID_POS) ? DEFAULT_XPOS : m_xpos;
    int ypos = (m_ypos == INVALID_POS) ? DEFAULT_YPOS : m_ypos;
    long style = m_fullscreen ? wxDEFAULT_FRAME_STYLE : wxSIMPLE_BORDER;

    m_mainWindow = new MainLayout(nullptr,
                              wxID_ANY,
                              m_disableMouse,
                              wxPoint(xpos, ypos),
                              wxSize(width, height),
                              wxDEFAULT_FRAME_STYLE);

    // m_mainWindow->GetWindowStyle() & ~wxSTAY_ON_TOP to remove it from top
    if(m_stayOnTop)
        m_mainWindow->SetWindowStyle(m_mainWindow->GetWindowStyle() | wxSTAY_ON_TOP);

    if(!m_fullscreen)
        m_mainWindow->Show(true);
    else
        m_mainWindow->ShowFullScreen(true);
}
