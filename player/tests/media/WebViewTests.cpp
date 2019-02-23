#include "WebViewTests.hpp"

using namespace ::testing;

TEST_F(WebViewTest, Construct_Default_HandlerLoadShouldBeCalled)
{
    FilePath path = DEFAULT_RESOURCES_DIR / FilePath(std::to_string(DEFAULT_ID) + ".html");

    EXPECT_CALL(adaptor(), load(path));

    constructWebView();
}

TEST_F(WebViewTest, Construct_NoTransparency_HandlerEnableTransparencyShouldBeCalled)
{
    EXPECT_CALL(adaptor(), enableTransparency());

    constructWebView({});
}

TEST_F(WebViewTest, Construct_TransparencyEnabled_HandlerEnableTransparencyShouldBeCalled)
{
    EXPECT_CALL(adaptor(), enableTransparency());

    constructWebView(WebViewOptions::Transparency::Enable);
}

TEST_F(WebViewTest, Construct_TransparentDisabled_HandlerEnableTransparencyShouldNotBeCalled)
{
    EXPECT_CALL(adaptor(), enableTransparency()).Times(0);

    constructWebView(WebViewOptions::Transparency::Disable);
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
