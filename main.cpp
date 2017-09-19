#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "PlayerApp.hpp"

#include <wx/xml/xml.h>
#include <vector>

//wxIMPLEMENT_APP(PlayerApp);


struct Transition
{
    enum Type
    {
        FadeIn,
        FadeOut,
        Fly
    } type;

    enum Direction
    {
        N,
        NE,
        E,
        SE,
        S,
        SW,
        W,
        NW
    } direction;

    int duration;
};

enum class MediaType
{
    Video,
    Image,
    Test
};

struct XiboObject
{
};

struct Media : XiboObject
{
    virtual void s() {}
    enum Render
    {
        HTML,
        Native
    } render;

    int id;
    int duration;

    struct BaseOptions
    {
        wxString uri;
    } options;
};

struct Video : Media
{
    struct VideoOptions : Media::BaseOptions
    {
        bool mute;
    } options;
};

struct MediaFactory
{
    Media* createMedia(const wxString& type)
    {
        if(type == "image")
        {
            return new Media;
        }
        else if(type == "video")
        {
            return new Video;
        }
        else if(type == "audio")
        {
            return new Media;
        }
        return new Media;
    }
};

struct Region : XiboObject
{
    int id;
    int width;
    int height;
    int top;
    int left;
    int zindex;

    struct Options
    {
        bool loop;
        Transition transition;
    } options;

    std::vector<Media*> medias;
};

struct Layout : XiboObject
{
    int schemaVersion;
    int width;
    int height;
    wxString backgroundImage;
    wxString backgroundColor;

    std::vector<Region*> regions;
};

struct Parser
{
    Parser(wxXmlNode* _node) : node(_node) { }
    virtual XiboObject* Parse() = 0;
    virtual XiboObject* GetAttributes(wxXmlNode* node) = 0;
    wxXmlNode* node;
};

template <typename Parser> Parser* GetParser(wxXmlNode* node)
{
    return new Parser{node};
}

struct MediaParser : Parser
{
    MediaParser(wxXmlNode* _node) : Parser(_node) { }

    Media* Parse() override
    {
        auto media = GetAttributes(node);

        auto currentNode = node->GetChildren();
        while(currentNode)
        {
            if(currentNode->GetName() == "options")
            {
                ParseOptions(media, currentNode->GetChildren());
            }
            currentNode = currentNode->GetNext();
        }

        return media;
    }

    void ParseOptions(Media* media, wxXmlNode* node)
    {
        auto currentNode = node;
        while(currentNode)
        {
            if(currentNode->GetName() == "uri")
            {
                media->options.uri = currentNode->GetChildren()->GetContent();
            }
            else
            {
                //
            }
            currentNode = currentNode->GetNext();
        }
    }

    Media* GetAttributes(wxXmlNode* node) override
    {
        std::cout << "parse media" << std::endl;
        auto media = MediaFactory().createMedia(node->GetAttribute("type"));
        media->id = std::stoi(node->GetAttribute("id").ToStdString());
        media->duration = std::stoi(node->GetAttribute("duration").ToStdString());
        //media->render = std::stoi(node->GetAttribute("render").ToStdString())
        return media;
    }
};

struct RegionParser : Parser
{
    RegionParser(wxXmlNode* _node) : Parser(_node) { }

    Region* Parse() override
    {
        auto region = GetAttributes(node);

        auto currentNode = node->GetChildren();
        while(currentNode)
        {
            if(currentNode->GetName() == "media")
            {
                auto media = GetParser<MediaParser>(currentNode)->Parse();
                region->medias.push_back(media);
            }
            else if(currentNode->GetName() == "options")
            {
                //auto options = ParseOptions();
            }
            currentNode = currentNode->GetNext();
        }

        return region;
    }

    Region* GetAttributes(wxXmlNode* node) override
    {
        std::cout << "parse region" << std::endl;
        Region* region = new Region;
        region->id = std::stoi(node->GetAttribute("id").ToStdString());
        region->width = std::stoi(node->GetAttribute("width").ToStdString());
        region->height = std::stoi(node->GetAttribute("height").ToStdString());
        region->top = std::stoi(node->GetAttribute("top").ToStdString());
        region->left = std::stoi(node->GetAttribute("left").ToStdString());
        return region;
    }
};


struct LayoutParser : Parser
{
    LayoutParser(wxXmlNode* _node) : Parser(_node) { }

    Layout* Parse() override
    {
        auto layout = GetAttributes(node);

        auto currentNode = node->GetChildren();
        while(currentNode)
        {
            if(currentNode->GetName() == "region")
            {
                auto region = GetParser<RegionParser>(currentNode)->Parse();
                layout->regions.push_back(region);
            }
            currentNode = currentNode->GetNext();
        }

        return layout;
    }

    Layout* GetAttributes(wxXmlNode* node) override
    {
        std::cout << "parse layout" << std::endl;
        Layout* layout = new Layout;
        layout->schemaVersion = std::stoi(node->GetAttribute("schemaVersion").ToStdString());
        layout->width = std::stoi(node->GetAttribute("width").ToStdString());
        layout->height = std::stoi(node->GetAttribute("height").ToStdString());
        layout->backgroundImage = node->GetAttribute("background");
        layout->backgroundColor = node->GetAttribute("bgcolor");
        return layout;
    }
};


int main()
{
    wxXmlDocument doc("85.xlf");
    wxXmlNode* root = doc.GetRoot();    

    auto layout = GetParser<LayoutParser>(root)->Parse();
    std::cout << layout->schemaVersion << " " << layout->width << " " << layout->height << " " << layout->backgroundColor << " " << layout->backgroundImage << std::endl;
    for(auto region : layout->regions)
    {
        std::cout << region->id << " " << region->width << " " << region->height << " " << region->top << " " << region->left << std::endl;
        for(auto media : region->medias)
        {
            if(auto video = dynamic_cast<Video*>(media))
                std::cout << video->options.mute << " ";
            std::cout << media->id << " " << media->duration << " " << media->options.uri << std::endl;
        }
    }

    return 0;
}
