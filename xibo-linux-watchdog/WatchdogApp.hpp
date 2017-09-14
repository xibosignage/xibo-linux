#ifndef XIBOWATCHDOGAPP_HPP
#define XIBOWATCHDOGAPP_HPP

#include <wx/app.h>

class WatchdogApp : public wxApp
{
public:
    WatchdogApp();

    bool OnInit() override;
    int OnExit() override;
};

#endif // XIBOWATCHDOGAPP_HPP
