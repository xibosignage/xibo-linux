#include "AbstractMediaBuilder.hpp"

#include "adaptors/IWebViewAdaptor.hpp"
#include "media/WebView.hpp"
#include "options/WebViewOptions.hpp"
#include "utils/Uri.hpp"

class WebViewBuilder;

template<>
struct BuilderTraits<WebViewBuilder>
{
    using Component = WebView;
    using Handler = IWebViewAdaptor;
    using Options = WebViewOptions;
};

const WebViewOptions::Transparency DEFAULT_TRANSPARENCY = WebViewOptions::Transparency::Enable;
const WebViewOptions::Mode DEFAULT_WEBVIEW_MODE = WebViewOptions::Mode::FileResource;
const std::string DEFAULT_WEBVIEW_EXTENSION = ".html";

class WebViewBuilder : public AbstractMediaBuilder<WebViewBuilder>
{
public:
    WebViewBuilder& width(int width);
    WebViewBuilder& height(int height);

protected:
    void retrieveMediaOptions(const WebViewOptions& opts) override;
    std::unique_ptr<WebView> create() override;
    std::unique_ptr<IWebViewAdaptor> createDefaultHandler() override;
    void doMediaSetup(WebView& webview) override;

private:
    Uri getUriOption(const boost::optional<std::string>& uriOpt) override;
    int getDurationOption(int duration) override;
    WebViewOptions::Transparency getTransparentOption(const boost::optional<WebViewOptions::Transparency>& transparentOpt);
    WebViewOptions::Mode getModeOption(const boost::optional<WebViewOptions::Mode>& modeOpt);

    boost::optional<int> parseDuration(const FilePath& path);
    std::string removeEscapedSymbolsFromUri(std::string url);

private:
    int m_width;
    int m_height;
    WebViewOptions::Mode m_mode;
    WebViewOptions::Transparency m_transparency;

};
