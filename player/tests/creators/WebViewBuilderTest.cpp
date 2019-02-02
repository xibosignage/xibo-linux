#include "WebViewBuilderTest.hpp"
#include "media/IMedia.hpp"

using namespace testing;

//TEST(WebViewBuilderTest, Construct_Default_HandlerLoadShouldBeCalled)
//{
//    WebViewBuilderTest builder;

//    FilePath path = DEFAULT_RESOURCES_DIR / FilePath("0.html");
//    EXPECT_CALL(builder.adaptor(), load(path));

//    builder.build();
//}

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
