#include "WebViewTests.hpp"

#include "utils/UriParseError.hpp"

using namespace ::testing;

const FilePath DEFAULT_WEBVIEW_PATH = DEFAULT_RESOURCES_DIR / FilePath(std::to_string(DEFAULT_ID) + DEFAULT_WEBVIEW_EXTENSION);
const Uri DEFAULT_WEBSITE_URI{Uri::Scheme::HTTP, "test.com", "/test"};

TEST_F(WebViewTest, Construct_NoWebViewMode_HandlerLoadFileResourceShouldBeCalled)
{
    WebViewOptions opts{DEFAULT_ID, std::string{}, DEFAULT_DURATION, DEFAULT_TRANSPARENCY, {}};

    EXPECT_CALL(adaptor(), load(Uri{Uri::Scheme::File, DEFAULT_WEBVIEW_PATH}));

    constructWebView(opts);
}

TEST_F(WebViewTest, Construct_FileResourceMode_HandlerLoadFileResourceShouldBeCalled)
{
    WebViewOptions opts{DEFAULT_ID, std::string{}, DEFAULT_DURATION, DEFAULT_TRANSPARENCY, WebViewOptions::Mode::FileResource};

    EXPECT_CALL(adaptor(), load(Uri{Uri::Scheme::File, DEFAULT_WEBVIEW_PATH}));

    constructWebView(opts);
}

TEST_F(WebViewTest, Construct_WebBrowserModeValidUrl_HandlerLoadFileResourceShouldBeCalled)
{
    WebViewOptions opts{DEFAULT_ID, DEFAULT_WEBSITE_URI.string(), DEFAULT_DURATION, DEFAULT_TRANSPARENCY, WebViewOptions::Mode::WebBrowser};

    EXPECT_CALL(adaptor(), load(DEFAULT_WEBSITE_URI));

    constructWebView(opts);
}

TEST_F(WebViewTest, Construct_WebBrowserInvalidUrl_HandlerLoadFileResourceShouldBeCalled)
{
    WebViewOptions opts{DEFAULT_ID, std::string{"invalid"}, DEFAULT_DURATION, DEFAULT_TRANSPARENCY, WebViewOptions::Mode::WebBrowser};

    ASSERT_THROW(constructWebView(opts), UriParseError);
}

TEST_F(WebViewTest, Construct_NoTransparency_HandlerEnableTransparencyShouldBeCalled)
{
    WebViewOptions opts{DEFAULT_ID, DEFAULT_URI.string(), DEFAULT_DURATION, {}, DEFAULT_WEBVIEW_MODE};

    EXPECT_CALL(adaptor(), enableTransparency());

    constructWebView(opts);
}

TEST_F(WebViewTest, Construct_TransparencyEnabled_HandlerEnableTransparencyShouldBeCalled)
{
    WebViewOptions opts{DEFAULT_ID, DEFAULT_URI.string(), DEFAULT_DURATION, WebViewOptions::Transparency::Enable, DEFAULT_WEBVIEW_MODE};

    EXPECT_CALL(adaptor(), enableTransparency());

    constructWebView(opts);
}

TEST_F(WebViewTest, Construct_TransparentDisabled_HandlerEnableTransparencyShouldNotBeCalled)
{
    WebViewOptions opts{DEFAULT_ID, DEFAULT_URI.string(), DEFAULT_DURATION, WebViewOptions::Transparency::Disable, DEFAULT_WEBVIEW_MODE};

    EXPECT_CALL(adaptor(), enableTransparency()).Times(0);

    constructWebView(opts);
}

TEST_F(WebViewTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructWebView();
}

TEST_F(WebViewTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto webview = constructWebView();

    ASSERT_EQ(&webview->handler(), &adaptor());
}

TEST_F(WebViewTest, Construct_Default_ScaleTypeScaledAlignLeftValignTop)
{
    auto webview = constructWebView();

    ASSERT_EQ(webview->scaleType(), MediaGeometry::ScaleType::Scaled);
    ASSERT_EQ(webview->align(), MediaGeometry::Align::Left);
    ASSERT_EQ(webview->valign(), MediaGeometry::Valign::Top);
}

TEST_F(WebViewTest, Width_HandlerReturnsDefaultWidth_WebViewWidthEqualsDefault)
{
    auto webview = constructWebView();

    ON_CALL(adaptor(), width()).WillByDefault(Return(DEFAULT_WIDTH));
    ON_CALL(adaptor(), height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(webview->width(), DEFAULT_WIDTH);
    ASSERT_EQ(webview->height(), DEFAULT_HEIGHT);
}

TEST_F(WebViewTest, Scale_Default_WebViewScaleShouldBeCalled)
{
    auto webview = constructWebView();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    webview->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(WebViewTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto webview = constructWebView();

    EXPECT_CALL(adaptor(), show());

    webview->show();
}

TEST_F(WebViewTest, Show_Default_HandlerReloadShouldBeCalled)
{
    auto webview = constructWebView();

    EXPECT_CALL(adaptor(), reload());

    webview->show();
}

TEST_F(WebViewTest, Hide_Default_HandlerHideShouldBeCalled)
{
    auto webview = constructWebView();

    EXPECT_CALL(adaptor(), hide());

    webview->hide();
}

TEST_F(WebViewTest, HandlerEvent_StartMediaEvent_HandlerShowShouldBeCalled)
{
    auto webview = constructWebView();

    EXPECT_CALL(adaptor(), show());

    webview->handleEvent(StartMediaEvent{});
}

TEST_F(WebViewTest, HandlerEvent_StopMediaEvent_HandlerHideShouldBeCalled)
{
    auto webview = constructWebView();

    EXPECT_CALL(adaptor(), hide());

    webview->handleEvent(StopMediaEvent{});
}

TEST_F(WebViewTest, HandlerEvent_ScaledMediaEvent_HandlerScaleShouldBeCalled)
{
    auto webview = constructWebView();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    webview->handleEvent(ScaleMediaEvent{DEFAULT_XSCALE, DEFAULT_YSCALE});
}
