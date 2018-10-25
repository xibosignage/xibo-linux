#pragma once

#include "BaseTestWithHandler.hpp"

#include "control/Background.hpp"
#include "mocks/MockImageAdaptor.hpp"

const uint32_t DEFAULT_COLOR = 255;

template<typename T>
auto constructBackground(std::unique_ptr<MockImageAdaptor>&& adaptor);

template<>
auto constructBackground<OneColorBackground>(std::unique_ptr<MockImageAdaptor>&& adaptor)
{
    return construct<OneColorBackground>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_COLOR, std::move(adaptor));
}

template<>
auto constructBackground<ImageBackground>(std::unique_ptr<MockImageAdaptor>&& adaptor)
{
    return construct<ImageBackground>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, std::move(adaptor));
}

template<typename T>
class BackgroundTest : public BaseTestWithHandler<MockImageAdaptor>
{
public:
    auto constructBackground()
    {
        if constexpr(std::is_same_v<T, OneColorBackground>)
        {
            return construct<OneColorBackground>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_COLOR, unique(&this->adaptor()));
        }
        else
        {
            return construct<ImageBackground>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, unique(&this->adaptor()));
        }
    }

};
