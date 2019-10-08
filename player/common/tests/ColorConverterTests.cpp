#include "ColorConverterTests.hpp"

#include "common/types/internal/ColorToHexConverter.hpp"

const std::vector<ColorTest> validColors = {{"#000", 255},
                                            {"#FFF", 4294967295},
                                            {"#123", 287454207},
                                            {"#1FA", 301968127},
                                            {"#fff", 4294967295},
                                            {"#fFf", 4294967295},
                                            {"#000000", 255},
                                            {"#12Ff32", 318714623},
                                            {"#4312AA", 1125296895},
                                            {"#aaFFbb", 2868886527},
                                            {"#FfFfFf", 4294967295},
                                            {"#000000FF", 255},
                                            {"#00fd00FF", 16580863},
                                            {"#FFFFFFFF", 4294967295},
                                            {"#FFF000ff", 4293918975},
                                            {"#FFFF", 4294967295},
                                            {"#123F", 287454207},
                                            {"#1234", 287454020}};

const std::vector<std::string> invalidColors = {"#sss",
                                                "121",
                                                "asdfasdf",
                                                "11G",
                                                "#12H",
                                                "#111V",
                                                "#11",
                                                "#1",
                                                "1",
                                                "#dfdsaw",
                                                "#11111",
                                                "#1111111"
                                                "#111111111",
                                                "zzzzzz",
                                                "test me",
                                                "invalid",
                                                "#dfdfd1s",
                                                "#1111d",
                                                "",
                                                " ",
                                                "   "};

TEST_P(ColorConverterTestsValidColorTest, ValidColor)
{
    ColorToHexConverter converter;
    auto colorHex = converter.colorToHex(GetParam().colorStr);

    EXPECT_EQ(colorHex, GetParam().colorHex);
}

INSTANTIATE_TEST_CASE_P(Suite, ColorConverterTestsValidColorTest, ::testing::ValuesIn(validColors));

TEST_P(ColorConverterTestsInvalidColorTest, InvalidColorShouldThrowException)
{
    ColorToHexConverter converter;
    ASSERT_THROW(converter.colorToHex(GetParam()), ColorToHexConverter::Error);
}

INSTANTIATE_TEST_CASE_P(Suite, ColorConverterTestsInvalidColorTest, ::testing::ValuesIn(invalidColors));
