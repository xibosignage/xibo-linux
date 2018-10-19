#include "WebView.hpp"

#include "constants.hpp"
#include "media/MediaVisitor.hpp"

#include <cassert>

WebView::WebView(std::unique_ptr<IWebViewAdaptor>&& handler) :
    m_handler(std::move(handler))
{
    assert(m_handler);
}

void WebView::doStop()
{
    m_handler->hide();
}

void WebView::doStart()
{
    m_handler->show();
    m_handler->reload();
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

void WebView::apply(MediaVisitor& visitor)
{
    visitor.visit(*this);
}

IWidgetAdaptor& WebView::handler()
{
    return *m_handler;
}
