#include "MediaBuilder.hpp"

#include "adaptors/IWebViewAdaptor.hpp"
#include "media/WebView.hpp"
#include "parsers/WebViewOptions.hpp"

class WebViewBuilder;

template<>
struct BuilderTraits<WebViewBuilder>
{
    using Media = WebView;
    using MediaHandler = IWebViewAdaptor;
    using Options = ResourcesXlf::WebViewOptions;
};

class WebViewBuilder : public BaseMediaBuilder<WebViewBuilder>
{
public:
    WebViewBuilder& width(int width);
    WebViewBuilder& height(int height);

protected:
    WebViewBuilder& mediaOptions(const ResourcesXlf::WebViewOptions& opts) override;
    std::unique_ptr<WebView> create() override;
    std::unique_ptr<IWebViewAdaptor> createHandler() override;
    void doSetup(WebView& webview) override;

private:
    FilePath getPathOption(const boost::optional<std::string>& pathOpt) override;
    int getDurationOption(int duration) override;
    boost::optional<int> parseDuration(const FilePath& path);
    bool getTransparentOption(const boost::optional<bool>& transparentOpt);

private:
    int m_width;
    int m_height;
    bool m_transparent;

};
