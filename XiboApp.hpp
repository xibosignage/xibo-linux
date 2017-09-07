#ifndef MYAPP_HPP
#define MYAPP_HPP

#include <wx/app.h>
#include "MainWindow.hpp"

class XiboApp : public wxApp
{
public:
    XiboApp();

    bool OnInit() override;
    int OnExit() override;

private:
    MainWindow* m_mainWindow = nullptr;

};

#endif // MYAPP_HPP
