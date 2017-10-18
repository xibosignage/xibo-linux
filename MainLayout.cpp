#include "MainLayout.hpp"
#include "Image.hpp"
#include "Video.hpp"
#include "Region.hpp"
#include "WebViewWrapper.hpp"

#include <wx/frame.h>
#include <wx/mediactrl.h>
#include <wx/dcclient.h>
#include <wx/webview.h>

#include <iostream>

MainLayout::MainLayout(wxWindow* parent,
               wxWindowID id,
               bool disableMouse,
               const wxPoint& pos,
               const wxSize& size,
               long style) :
    wxFrame(parent, id, wxString{}, pos, size, style),
    m_backgroundImage("back.jpg")
{
    if(disableMouse)
        SetCursor(wxCursor(wxCURSOR_BLANK));

    Bind(wxEVT_PAINT, &MainLayout::OnPaint, this);


    auto region1 = new Region{this, wxPoint(420, 420),  wxSize(150, 150), 2, 1};
    region1->AddMedia({std::make_shared<Image>("1.jpg"),
                       std::make_shared<Image>("2.jpg"),
                       std::make_shared<Image>("3.jpg"),
                       std::make_shared<Image>("4.png")});

    auto image = dynamic_cast<Image*>(region1->m_medias[region1->m_currentIndex].get());
    image->m_imageRender->Bind(TEST_PAINT, &MainLayout::Test, this);

    auto region2 = new Region{this, wxPoint(285, 285),  wxSize(150, 150), 2, 2};
    region2->AddMedia({std::make_shared<Image>("2.jpg"),
                       std::make_shared<Image>("3.jpg"),
                       std::make_shared<Image>("4.png"),
                       std::make_shared<Image>("1.jpg")});

    auto image2 = dynamic_cast<Image*>(region2->m_medias[region2->m_currentIndex].get());
    image2->m_imageRender->Bind(TEST_PAINT, &MainLayout::Test, this);

    auto region3 = new Region{this, wxPoint(150, 150),  wxSize(150, 150), 2, 3};
    region3->AddMedia({std::make_shared<Image>("3.jpg"),
                       std::make_shared<Image>("4.png"),
                       std::make_shared<Image>("1.jpg"),
                       std::make_shared<Image>("2.jpg")});

    auto image3 = dynamic_cast<Image*>(region3->m_medias[region3->m_currentIndex].get());
    image3->m_imageRender->Bind(TEST_PAINT, &MainLayout::Test, this);

    auto region4 = new Region{this, wxPoint(15, 15),  wxSize(150, 150), 2, 4};
    region4->AddMedia({std::make_shared<Image>("4.png"),
                       std::make_shared<Image>("1.jpg"),
                       std::make_shared<Image>("2.jpg"),
                       std::make_shared<Image>("3.jpg")});

    auto image4 = dynamic_cast<Image*>(region4->m_medias[region4->m_currentIndex].get());
    image4->m_imageRender->Bind(TEST_PAINT, &MainLayout::Test, this);

    auto region5 = new Region{this, wxPoint(500, 15),  wxSize(150, 150), 2, 5};
    region5->AddMedia(std::make_shared<Image>("4.png"));

    auto image5 = dynamic_cast<Image*>(region5->m_medias[region5->m_currentIndex].get());
    image5->m_imageRender->Bind(TEST_PAINT, &MainLayout::Test, this);

//    auto region = new Region{this, wxPoint(400, 400),  wxSize(400, 400), 2};
//    region->AddMedia({std::make_shared<Video>("video.mp4", false, true)});

//    auto wrapper = new WebViewWrapper(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, 200));
//    wrapper->webView()->LoadURL("file:///home/stivius/build-xibo-linux-Desktop_Qt_5_9_2_GCC_64bit-Debug/test.htm");
}


void MainLayout::Test(wxCommandEvent& ev)
{
    std::cout << "posted3 " << ev.GetString() << std::endl;
}


void MainLayout::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    dc.SetBrush(wxBrush(wxColour("black")));
    dc.DrawRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight());
    dc.DrawBitmap(wxBitmap(m_backgroundImage), 0, 0, false);
}
