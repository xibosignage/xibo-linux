#pragma once

#include <utility>
#include "common/PlayerError.hpp"

template<typename Result>
using ResponseResult = std::pair<PlayerError, Result>;
