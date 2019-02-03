#include "MediaBuilder.hpp"

#include "adaptors/IWebViewAdaptor.hpp"
#include "media/WebView.hpp"
#include "parsers/WebViewOptions.hpp"

class WebViewBuilder : public BaseMediaBuilder<WebViewBuilder, ResourcesXlf::WebViewOptions>
{
public:
    std::unique_ptr<WebView> build();

    WebViewBuilder& width(int width);
    WebViewBuilder& height(int height);

protected:
    virtual std::unique_ptr<IWebViewAdaptor> createAdaptor();
    WebViewBuilder& mediaOptions(const ResourcesXlf::WebViewOptions& opts) override;

private:
    std::unique_ptr<WebView> createWebView();

    FilePath getPathOption(const boost::optional<std::string>& pathOpt) override;
    int getDurationOption(int duration) override;
    boost::optional<int> parseDuration(const FilePath& path);
    bool getTransparentOption(const boost::optional<bool>& transparentOpt);

private:
    int m_width;
    int m_height;
    bool m_transparent;

};
