#include "control/MainLayout.hpp"
#include "control/IOverlayWrapper.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gtkmm/fixed.h>
#include <gtkmm/image.h>

using namespace ::testing;

class MockOverlayWrapper : public IOverlayWrapper
{
public:
    MOCK_METHOD0(show, void());
    MOCK_METHOD2(set_size, void(int width, int height));
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD5(add_child, void(Gtk::Fixed& child, int top, int left, int width, int height));
    MOCK_METHOD1(add, void(Gtk::Image& child));
    MOCK_METHOD0(remove, void());
    MOCK_METHOD2(reorder_overlay, void(Gtk::Fixed& child, int position));
};

TEST(MainLayout, RegionsCount_Default_Equals0)
{
    auto wrapper = std::make_shared<MockOverlayWrapper>();
    MainLayout layout(640, 480, wrapper);

    ASSERT_EQ(layout.regions_count(), 0);
}

TEST(MainLayout, Width_Default_Equals640)
{
    auto wrapper = std::make_shared<MockOverlayWrapper>();
    MainLayout layout(640, 480, wrapper);

    EXPECT_CALL(*wrapper, width()).WillOnce(Return(640));
    ASSERT_EQ(layout.width(), 640);
}

TEST(MainLayout, Height_Default_Equals480)
{
    auto wrapper = std::make_shared<MockOverlayWrapper>();
    MainLayout layout(640, 480, wrapper);

    EXPECT_CALL(*wrapper, height()).WillOnce(Return(480));
    ASSERT_EQ(layout.height(), 480);
}

TEST(MainLayout, Background_Default_ShouldThrowRunTimeException)
{
    auto wrapper = std::make_shared<MockOverlayWrapper>();
    MainLayout layout(640, 480, wrapper);

    ASSERT_THROW(layout.background(), std::runtime_error);
}

TEST(MainLayout, Region_DefaultGet0Element_ShouldThrowOutOfRangeException)
{
    auto wrapper = std::make_shared<MockOverlayWrapper>();
    MainLayout layout(640, 480, wrapper);

    ASSERT_THROW(layout.region(0), std::out_of_range);
}

TEST(MainLayout, Show_Default_HandlerShowShouldBeCalled)
{
    auto wrapper = std::make_shared<MockOverlayWrapper>();
    MainLayout layout(640, 480, wrapper);

    EXPECT_CALL(*wrapper, show);

    layout.show();
}
