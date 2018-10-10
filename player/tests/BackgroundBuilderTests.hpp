#pragma once

#include "test_utils.hpp"

#include "creators/BackgroundBuilder.hpp"

#include "mocks/MockBackground.hpp"
#include "mocks/MockImageAdaptor.hpp"

struct Color
{
    std::string strColor;
    uint numberColor;
};

const auto invalidBackgroundSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, MIN_DISPLAY_HEIGHT>;

class BackgroundBuilderSizeTest : public testing::TestWithParam<Size> { };
class BackgroundBuilderValidColorTest : public testing::TestWithParam<Color> { };
class BackgroundBuilderInvalidColorTest :  public testing::TestWithParam<std::string> { };

const std::vector<Color> validColors =
    {{"#000", 255}, {"#FFF", 4294967295}, {"#123", 287454207}, {"#1FA", 301968127}, {"#fff", 4294967295}, {"#fFf", 4294967295},
     {"#000000", 255}, {"#12Ff32", 318714623}, {"#4312AA", 1125296895}, {"#aaFFbb", 2868886527}, {"#FfFfFf", 4294967295},
     {"#000000FF", 255}, {"#00fd00FF", 16580863}, {"#FFFFFFFF", 4294967295}, {"#FFF000ff", 4293918975}, {"#FFFF", 4294967295},
     {"#123F", 287454207}, {"#1234", 287454020}};

const std::vector<std::string> invalidColors =
    {"#sss", "121", "asdfasdf", "11G", "#12H", "#111V",
    "#11", "#1", "1", "#dfdsaw", "#11111", "#1111111"
    "#111111111", "zzzzzz", "test me", "invalid",
    "#dfdfd1s", "#1111d", "", " ", "   "};

const std::string DEFAULT_PATH = "test";
const std::string DEFAULT_COLOR = "#000";

class BackgroundBuilderTest : public BackgroundBuilder
{
public:
    BackgroundBuilderTest() :
        BackgroundBuilder(std::make_unique<MockFileSystemAdaptor>())
    {
    }

    BackgroundBuilderTest(std::unique_ptr<MockFileSystemAdaptor>&& filesystem) :
        BackgroundBuilder(std::move(filesystem))
    {
    }

    BackgroundBuilderTest& defaultAdaptor()
    {
        return static_cast<BackgroundBuilderTest&>(adaptor(std::make_unique<testing::NiceMock<MockImageAdaptor>>()));
    }

    BackgroundBuilderTest& defaultSize()
    {
        return static_cast<BackgroundBuilderTest&>(width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT));
    }

protected:
    std::unique_ptr<IBackground> createBackground() override
    {
        return fake_construct<MockBackground, MockImageAdaptor>();
    }

};
