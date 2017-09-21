#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "PlayerApp.hpp"

#include <wx/xml/xml.h>
#include <vector>

//wxIMPLEMENT_APP(PlayerApp);

#include "LayoutParser.hpp"
#include "Video.hpp"
#include "utilities.hpp"

int main()
{
    wxXmlDocument doc("85.xlf");
    wxXmlNode* root = doc.GetRoot();

    auto layout = std::shared_ptr<Layout>(utilities::GetParser<LayoutParser>(root)->Parse());
    std::cout << layout->schemaVersion << " " << layout->width << " " << layout->height << " " << layout->backgroundColor << " " << layout->backgroundImage << std::endl;
    for(auto region : layout->regions)
    {
        std::cout << "region " << region->id << " " << region->width << " " << region->height << " " << region->top << " " << region->left << " " << region->options.loop << std::endl;
        for(auto media : region->medias)
        {
            if(auto video = dynamic_cast<Video*>(media.get()))
                std::cout << video->options.mute << " ";
            std::cout << media->id << " " << media->duration << " " << media->options.uri << std::endl;
        }
    }

    return 0;
}
