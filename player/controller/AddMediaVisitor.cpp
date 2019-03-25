#include "AddMediaVisitor.hpp"

#include "model/GetMediaPosition.hpp"
#include "model/ImageModel.hpp"
#include "model/WebViewModel.hpp"
#include "model/VideoModel.hpp"
#include "model/AudioModel.hpp"

#include "controller/AudioController.hpp"
#include "controller/VideoController.hpp"
#include "controller/VisibleMediaController.hpp"

#include "view/WebView.hpp"
#include "view/DrawingArea.hpp"
#include "view/Image.hpp"

AddMediaVisitor::AddMediaVisitor(FixedLayout& view, RegionController& controller) :
    m_view(view), m_controller(controller)
{
}

void AddMediaVisitor::visit(const std::shared_ptr<ImageModel>& imageModel)
{
    auto image = std::make_shared<Image>(m_view.width(), m_view.height());
    image->loadFromFile(imageModel->uri(), imageModel->isScaled());

    GetMediaPosition positionCalc{m_view.width(), m_view.height()};
    int left = positionCalc.getMediaLeft(image->width(), imageModel->align());
    int top = positionCalc.getMediaTop(image->height(), imageModel->valign());

    m_view.addWidget(image, left, top);
    m_controller.addMedia(std::make_unique<VisibleMediaController>(imageModel, image));
}

void AddMediaVisitor::visit(const std::shared_ptr<WebViewModel>& webviewModel)
{
    auto webview = std::make_shared<WebView>(m_view.width(), m_view.height());

    webview->load(webviewModel->uri());

    if(webviewModel->transparency() == WebViewModel::Transparency::Enable)
    {
        webview->enableTransparency();
    }

    m_view.addWidget(webview, DEFAULT_XPOS, DEFAULT_YPOS);
    m_controller.addMedia(std::make_unique<VisibleMediaController>(webviewModel, webview));
}

void AddMediaVisitor::visit(const std::shared_ptr<AudioModel>& audioModel)
{
   m_controller.addMedia(std::make_unique<AudioController>(audioModel));
}

void AddMediaVisitor::visit(const std::shared_ptr<VideoModel>& videoModel)
{
    auto videoWindow = std::make_shared<DrawingArea>(m_view.width(), m_view.height());

    m_view.addWidget(videoWindow, DEFAULT_XPOS, DEFAULT_YPOS);
    m_controller.addMedia(std::make_unique<VideoController>(videoModel, videoWindow));
}
