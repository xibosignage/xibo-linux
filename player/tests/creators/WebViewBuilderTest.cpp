#include "WebViewBuilderTest.hpp"

#include "media/WebView.hpp"

using namespace testing;

TEST(WebViewBuilderTest, Construct_Default_HandlerLoadShouldBeCalled)
{
    WebViewBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), load(DEFAULT_RESOURCES_DIR / FilePath("0.html")));

    builder.build();
}

TEST(WebViewBuilderTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    WebViewBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    builder.build();
}

TEST(WebViewBuilderTest, Construct_TransparentTrue_HandlerEnableTransparencyShouldBeCalled)
{
    WebViewBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), enableTransparency());

    builder.transparent(true).build();
}

TEST(WebViewBuilderTest, Construct_TransparentFalse_HandlerEnableTransparencyShouldNotBeCalled)
{
    WebViewBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), enableTransparency()).Times(0);

    builder.transparent(false).build();
}
