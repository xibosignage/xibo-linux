#pragma once

#include "BaseTestWithHandler.hpp"

#include "control/Background.hpp"
#include "mocks/MockImageAdaptor.hpp"

const uint32_t DEFAULT_COLOR = 255;

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
