#include "WebViewTests.hpp"

using namespace ::testing;

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

/* ----------------------- GENERAL MEDIA TESTS ----------------------- */

TEST(WebViewTest, Duration_Default_DurationEqualsDefault)
{
    auto [webview, webviewHandlerStub] = constructWebView();

    ASSERT_EQ(webview->duration(), DEFAULT_DURATION);
}

TEST(WebViewTest, SetDuration_NewValue_DurationEqualsNewValue)
{
    auto [webview, webviewHandlerStub] = constructWebView();

    webview->setDuration(DEFAULT_DURATION + 1);

    ASSERT_EQ(webview->duration(), DEFAULT_DURATION + 1);
}

TEST(WebViewTest, Start_WithAttachedMedia_AttachedMediaStartShouldBeCalled)
{
    auto [webview, webviewHandlerStub] = constructWebView();
    auto mockMedia = createMedia();
    webview->attachMedia(unique(mockMedia));

    EXPECT_CALL(*mockMedia, start());

    webview->start();
}

TEST(WebViewTest, Stop_WithAttachedMedia_AttachedMediaStopShouldBeCalled)
{
    auto [webview, webviewHandlerStub] = constructWebView();
    auto mockMedia = createMedia();
    webview->attachMedia(unique(mockMedia));

    EXPECT_CALL(*mockMedia, stop());

    webview->stop();
}

/* ----------------------- GENERAL MEDIA TESTS ----------------------- */
