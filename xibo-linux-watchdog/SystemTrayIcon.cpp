#include "SystemTrayIcon.hpp"

#include <wx/taskbar.h>
#include <wx/icon.h>

SystemTrayIcon::SystemTrayIcon()
{
    SetIcon(wxIcon("tray.png"));
}
