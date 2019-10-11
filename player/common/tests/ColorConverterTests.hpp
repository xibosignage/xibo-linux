#pragma once

#include <gtest/gtest.h>

struct ColorTest
{
    std::string colorStr;
    uint32_t colorHex;
};

class ColorConverterTestsValidColorTest : public testing::TestWithParam<ColorTest>
{
};
class ColorConverterTestsInvalidColorTest : public testing::TestWithParam<std::string>
{
};
