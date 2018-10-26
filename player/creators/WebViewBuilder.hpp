#include "MediaBuilder.hpp"

class IWebViewAdaptor;
class WebView;

class WebViewBuilder : public MediaBuilder
{
public:
    WebViewBuilder& path(const boost::optional<std::string>& path) override;
    WebViewBuilder& duration(int duration) override;

    WebViewBuilder& width(int width);
    WebViewBuilder& height(int height);
    WebViewBuilder& transparent(const boost::optional<bool>& transparent);

protected:
    std::unique_ptr<IMedia> doBuild() final;
    virtual std::unique_ptr<IWebViewAdaptor> createAdaptor();

private:
    std::unique_ptr<WebView> createWebView();
    boost::optional<int> parseDuration(const FilePath& path);

private:
    int m_width;
    int m_height;
    bool m_transparent;

};
