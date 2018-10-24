#include "MediaTests.hpp"

using MediaTypes = ::testing::Types<WebView, Image>;
TYPED_TEST_CASE(MediaTest, MediaTypes);

TYPED_TEST(MediaTest, Duration_Default_DurationEqualsDefault)
{
    auto [media, handlerStub] = constructMedia<TypeParam>();

    ASSERT_EQ(media->duration(), DEFAULT_DURATION);
}

TYPED_TEST(MediaTest, SetDuration_NewValue_DurationEqualsNewValue)
{
    auto [media, handlerStub] = constructMedia<TypeParam>();

    media->setDuration(DEFAULT_DURATION + 1);

    ASSERT_EQ(media->duration(), DEFAULT_DURATION + 1);
}

TYPED_TEST(MediaTest, Start_WithAttachedMedia_AttachedMediaStartShouldBeCalled)
{
    auto [media, handlerStub] = constructMedia<TypeParam>();
    auto mockMedia = createMedia();
    media->attachMedia(unique(mockMedia));

    EXPECT_CALL(*mockMedia, start());

    media->start();
}

TYPED_TEST(MediaTest, Stop_WithAttachedMedia_AttachedMediaStopShouldBeCalled)
{
    auto [media, handlerStub] = constructMedia<TypeParam>();
    auto mockMedia = createMedia();
    media->attachMedia(unique(mockMedia));

    EXPECT_CALL(*mockMedia, stop());

    media->stop();
}
