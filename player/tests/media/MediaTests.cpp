#include "MediaTests.hpp"

using MediaTypes = ::testing::Types<WebView, Image>;
TYPED_TEST_CASE(MediaTest, MediaTypes);

TYPED_TEST(MediaTest, Duration_Default_DurationEqualsDefault)
{
    auto media = constructMedia<TypeParam>();

    ASSERT_EQ(media->duration(), DEFAULT_DURATION);
}

TYPED_TEST(MediaTest, Id_Default_IdEqualsDefault)
{
    auto media = constructMedia<TypeParam>();

    ASSERT_EQ(media->id(), DEFAULT_ID);
}

TYPED_TEST(MediaTest, SetDuration_NewValue_DurationEqualsNewValue)
{
    auto media = constructMedia<TypeParam>();

    media->setDuration(DEFAULT_DURATION + 1);

    ASSERT_EQ(media->duration(), DEFAULT_DURATION + 1);
}
