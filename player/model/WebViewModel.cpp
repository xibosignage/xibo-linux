#include "WebViewModel.hpp"

WebViewModel::WebViewModel(const WebViewOptions& options) :
    MediaModel(options)
{
    m_transparency = static_cast<Transparency>(options.transparency);
}

WebViewModel::Transparency WebViewModel::transparency() const
{
    return m_transparency;
}

void WebViewModel::apply(IMediaVisitor& visitor)
{
    visitor.visit(shared_from_this());
}
