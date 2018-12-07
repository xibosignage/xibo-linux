#include "WebViewTests.hpp"

using namespace ::testing;

TEST_F(WebViewTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructWebView();
}

TEST_F(WebViewTest, Construct_Default_HandlerSetImageShouldBeCalled)
{
    EXPECT_CALL(adaptor(), load(DEFAULT_PATH));

    constructWebView();
}

TEST_F(WebViewTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto webview = constructWebView();

    ASSERT_EQ(&webview->handler(), &adaptor());
}

TEST_F(WebViewTest, Width_HandlerReturnsDefaultWidth_WebViewWidthEqualsDefault)
{
    auto webview = constructWebView();

    ON_CALL(adaptor(), width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(webview->width(), DEFAULT_WIDTH);
}

TEST_F(WebViewTest, Height_HandlerReturnsDefaultHeight_WebViewHeightEqualsDefault)
{
    auto webview = constructWebView();

    ON_CALL(adaptor(), height()).WillByDefault(Return(DEFAULT_HEIGHT));

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

TEST_F(WebViewTest, SetTransparent_True_HandlerEnableTransparencyShouldBeCalled)
{
    auto webview = constructWebView();

    EXPECT_CALL(adaptor(), enableTransparency());

    webview->setTransparent(true);
}

TEST_F(WebViewTest, SetTransparent_False_HandlerEnableTransparencyShouldNotBeCalled)
{
    auto webview = constructWebView();

    EXPECT_CALL(adaptor(), enableTransparency()).Times(0);

    webview->setTransparent(false);
}
