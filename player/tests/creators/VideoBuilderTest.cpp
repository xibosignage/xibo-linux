#include "VideoBuilderTest.hpp"

#include "media/IMedia.hpp"
#include "adaptors/IWidgetAdaptor.hpp"

using namespace testing;

TEST(VideoBuilderTest, Construct_Default_HandlerLoadShouldBeCalled)
{
    VideoBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), load(DEFAULT_FULL_PATH));

    builder.build();
}

TEST(VideoBuilderTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    VideoBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    builder.build();
}

TEST(VideoBuilderTest, Construct_MutedTrue_HandlerSetVolume0ShouldBeCalled)
{
    VideoBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), setVolume(MIN_VOLUME));

    builder.muted(true).build();
}

TEST(VideoBuilderTest, Construct_MutedTrue_HandlerSetVolume100ShouldBeCalled)
{
    VideoBuilderTest builder;

    EXPECT_CALL(builder.adaptor(), setVolume(MAX_VOLUME));

    builder.muted(false).build();
}

TEST(VideoBuilderTest, Construct_InvalidPath_ShouldThrowRunTimeError)
{
    VideoBuilderTest builder;

    ON_CALL(builder.filesystem(), isRegularFile(_)).WillByDefault(Return(false));

    ASSERT_THROW(builder.path(DEFAULT_PATH.string()).build(), std::runtime_error);
}
