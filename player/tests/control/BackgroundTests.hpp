#pragma once

#include "BaseTestWithHandler.hpp"

#include "control/Background.hpp"
#include "mocks/MockImageAdaptor.hpp"

const std::string DEFAULT_COLOR = "#fff";

#include "creators/BackgroundBuilderTests.hpp"

template<typename T>
class BackgroundTest : public BaseTestWithHandler<MockImageAdaptor>
{
public:
    auto constructBackground()
    {
        if constexpr(std::is_same_v<T, OneColorBackground>)
        {
            ResourcesXlf::BackgroundOptions opts{DEFAULT_WIDTH, DEFAULT_HEIGHT, {}, DEFAULT_COLOR};
            return BackgroundBuilderTest{}.adaptor(unique(&this->adaptor())).options(opts).build();
        }
        else
        {
            ResourcesXlf::BackgroundOptions opts{DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH.string(), {}};
            return BackgroundBuilderTest{}.adaptor(unique(&this->adaptor())).options(opts).build();
        }
    }

};
