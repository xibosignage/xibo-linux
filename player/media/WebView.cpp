#include "WebView.hpp"

#include "adaptors/IWebViewAdaptor.hpp"

#include <cassert>

WebView::WebView(int id, int width, int height, const FilePath& path, std::unique_ptr<IWebViewAdaptor>&& handler) :
    Media(id), m_handler(std::move(handler))
{
    assert(m_handler);

    m_handler->setSize(width, height);
    m_handler->load(path);
}

void WebView::show()
{
    m_handler->show();
    m_handler->reload();
}

void WebView::hide()
{
    m_handler->hide();
}

void WebView::scale(double scaleX, double scaleY)
{
    m_handler->scale(scaleX, scaleY);
}

int WebView::width() const
{
    return m_handler->width();
}

int WebView::height() const
{
    return m_handler->height();
}

void WebView::handleEvent(const Event& ev)
{
    switch(ev.type())
    {
        case EventType::StartMedia:
            show();
            break;
        case EventType::StopMedia:
            hide();
            break;
        case EventType::ScaleMedia:
        {
            auto scaleEv = static_cast<const ScaleMediaEvent&>(ev);
            scale(scaleEv.scaleX(), scaleEv.scaleY());
            break;
        }
        default:
            break;
    }
}

MediaGeometry::Align WebView::align() const
{
    return MediaGeometry::Align::Left;
}

MediaGeometry::Valign WebView::valign() const
{
    return MediaGeometry::Valign::Top;
}

MediaGeometry::ScaleType WebView::scaleType() const
{
    return MediaGeometry::ScaleType::Scaled;
}

void WebView::setTransparent(WebViewOptions::Transparency transparency)
{
    if(transparency == WebViewOptions::Transparency::Enable)
    {
        m_handler->enableTransparency();
    }
}

IWidgetAdaptor& WebView::handler()
{
    return *m_handler;
}
