#pragma once

#include "test_utils.hpp"

#include "control/Background.hpp"
#include "mocks/MockImageAdaptor.hpp"

inline auto constructBackground()
{
    return construct<Background, MockImageAdaptor>();
}
