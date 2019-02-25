#include "MediaTests.hpp"

//using MediaTypes = ::testing::Types<Audio>;
//TYPED_TEST_CASE(MediaTest, MediaTypes);


//TYPED_TEST(MediaTest, Duration_Default_DurationEqualsDefault)
//{
//    auto media = this->constructMedia();

//    ASSERT_EQ(media->duration(), DEFAULT_DURATION);
//}

//TYPED_TEST(MediaTest, Id_Default_IdEqualsDefault)
//{
//    auto media = this->constructMedia();

//    ASSERT_EQ(media->id(), DEFAULT_ID);
//}

//TYPED_TEST(MediaTest, SetDuration_NewValue_DurationEqualsNewValue)
//{
//    auto media = this->constructMedia();

//    media->setDuration(DEFAULT_DURATION + 1);

//    ASSERT_EQ(media->duration(), DEFAULT_DURATION + 1);
//}

//TYPED_TEST(MediaTest, Construct_InvalidPath_ShouldThrowRunTimeError)
//{
//    ON_CALL(filesystem(), isRegularFile(_)).WillByDefault(Return(false));

//    ASSERT_THROW(this->constructMedia(), std::runtime_error);
//}
