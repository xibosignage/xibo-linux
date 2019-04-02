#pragma once

#include "MediaModel.hpp"
#include "parsers/Options.hpp"

class WebViewModel : public MediaModel, public std::enable_shared_from_this<WebViewModel>
{
public:
    enum class Transparency
    {
        Disable,
        Enable
    };

    enum class Mode
    {
        FileResource,
        WebBrowser
    };

    WebViewModel(const WebViewOptions& options);

    void apply(IMediaVisitor& visitor) override;

    Transparency transparency() const;

private:
    Transparency m_transparency;

};
