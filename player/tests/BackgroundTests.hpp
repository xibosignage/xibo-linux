#pragma once

#include "test_utils.hpp"

#include "control/Background.hpp"
#include "mocks/MockImageAdaptor.hpp"

const uint32_t DEFAULT_COLOR = 255;

template<typename T>
auto constructBackground();

template<typename T>
auto constructBackground(std::unique_ptr<MockImageAdaptor>&& adaptor);

template<>
auto constructBackground<OneColorBackground>()
{
    return construct<OneColorBackground, MockImageAdaptor>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_COLOR);
}

template<>
auto constructBackground<OneColorBackground>(std::unique_ptr<MockImageAdaptor>&& adaptor)
{
    return construct<OneColorBackground>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_COLOR, std::move(adaptor));
}

template<>
auto constructBackground<ImageBackground>()
{
    return construct<ImageBackground, MockImageAdaptor>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH);
}

template<>
auto constructBackground<ImageBackground>(std::unique_ptr<MockImageAdaptor>&& adaptor)
{
    return construct<ImageBackground>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, std::move(adaptor));
}

template<typename T>
class BackgroundTest : public testing::Test
{
};
