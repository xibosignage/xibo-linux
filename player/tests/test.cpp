#include "control/MainLayout.hpp"
#include "control/Region.hpp"
#include "utils/utilities.hpp"

#include "gtest/gtest.h"

TEST(Utilities, ToHex)
{
    EXPECT_EQ(255, utilities::to_hex("#000"));
    EXPECT_EQ(255, utilities::to_hex("#000000"));
    EXPECT_EQ(255, utilities::to_hex("#000000FF"));
    EXPECT_EQ(301946367, utilities::to_hex("#1F5"));
    EXPECT_EQ(301946367, utilities::to_hex("#11FF55"));
    EXPECT_EQ(301946367, utilities::to_hex("#11FF55FF"));
}

TEST(MainLayout, Construction)
{
    MainLayout layout(0, 640, 480);

    int width, height;
    layout.get_size(width, height);

    EXPECT_EQ(layout.width(), 640);
    EXPECT_EQ(layout.height(), 480);
    EXPECT_EQ(layout.width_scale_factor(), 0.0);
    EXPECT_EQ(layout.height_scale_factor(), 0.0);
    EXPECT_EQ(width, 640);
    EXPECT_EQ(height, 480);
    EXPECT_EQ(layout.regions_count(), 0);
}
