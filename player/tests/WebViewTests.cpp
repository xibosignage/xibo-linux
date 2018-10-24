#include "WebViewTests.hpp"

using namespace ::testing;

TEST(WebViewTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    auto webviewHandlerMock = std::make_unique<NiceMock<MockWebViewAdaptor>>();

    EXPECT_CALL(*webviewHandlerMock, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructWebView(std::move(webviewHandlerMock));
}

TEST(WebViewTest, Construct_Default_HandlerSetImageShouldBeCalled)
{
    auto webviewHandlerMock = std::make_unique<NiceMock<MockWebViewAdaptor>>();

    EXPECT_CALL(*webviewHandlerMock, load(DEFAULT_PATH));

    constructWebView(std::move(webviewHandlerMock));
}

TEST(WebViewTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [webview, webviewHandlerStub] = constructWebView();

    ASSERT_EQ(&webview->handler(), webviewHandlerStub);
}

TEST(WebViewTest, Width_HandlerReturnsDefaultWidth_WebViewWidthEqualsDefault)
{
    auto [webview, webviewHandlerStub] = constructWebView();

    ON_CALL(*webviewHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(webview->width(), DEFAULT_WIDTH);
}

TEST(WebViewTest, Height_HandlerReturnsDefaultHeight_WebViewHeightEqualsDefault)
{
    auto [webview, webviewHandlerStub] = constructWebView();

    ON_CALL(*webviewHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(webview->height(), DEFAULT_HEIGHT);
}

TEST(WebViewTest, Scale_Default_WebViewScaleShouldBeCalled)
{
    auto [webview, webviewHandlerMock] = constructWebView();

    EXPECT_CALL(*webviewHandlerMock, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    webview->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST(WebViewTest, Start_Default_HandlerShowShouldBeCalled)
{
    auto [webview, webviewHandlerMock] = constructWebView();

    EXPECT_CALL(*webviewHandlerMock, show());

    webview->start();
}

TEST(WebViewTest, Start_Default_HandlerReloadShouldBeCalled)
{
    auto [webview, webviewHandlerMock] = constructWebView();

    EXPECT_CALL(*webviewHandlerMock, reload());

    webview->start();
}

TEST(WebViewTest, Stop_Default_HandlerHideShouldBeCalled)
{
    auto [webview, webviewHandlerMock] = constructWebView();

    EXPECT_CALL(*webviewHandlerMock, hide());

    webview->stop();
}

TEST(WebViewTest, SetTransparent_True_HandlerEnableTransparencyShouldBeCalled)
{
    auto [webview, webviewHandlerMock] = constructWebView();

    EXPECT_CALL(*webviewHandlerMock, enableTransparency());

    webview->setTransparent(true);
}

TEST(WebViewTest, SetTransparent_False_HandlerEnableTransparencyShouldNotBeCalled)
{
    auto [webview, webviewHandlerMock] = constructWebView();

    EXPECT_CALL(*webviewHandlerMock, enableTransparency()).Times(0);

    webview->setTransparent(false);
}
