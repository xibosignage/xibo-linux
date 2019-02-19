#include "MediaBuilder.hpp"

#include "adaptors/WebKitWebViewAdaptor.hpp"
#include "media/WebView.hpp"
#include "parsers/WebViewOptions.hpp"

class WebViewBuilder;

template<>
struct BuilderTraits<WebViewBuilder>
{
    using Component = WebView;
    using DefaultHandler = WebKitWebViewAdaptor;
    using Options = ResourcesXlf::WebViewOptions;
};

class WebViewBuilder : public AbstractMediaBuilder<WebViewBuilder>
{
public:
    WebViewBuilder& width(int width);
    WebViewBuilder& height(int height);

protected:
    WebViewBuilder& retrieveMediaOptions(const ResourcesXlf::WebViewOptions& opts) override;
    std::unique_ptr<WebView> create() override;
    void doMediaSetup(WebView& webview) override;

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
