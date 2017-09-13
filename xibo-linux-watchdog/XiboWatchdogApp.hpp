#ifndef XIBOWATCHDOGAPP_HPP
#define XIBOWATCHDOGAPP_HPP

#include <wx/app.h>

class XiboWatchdogApp : public wxApp
{
public:
    XiboWatchdogApp();

    bool OnInit() override;
    int OnExit() override;
};

#endif // XIBOWATCHDOGAPP_HPP
