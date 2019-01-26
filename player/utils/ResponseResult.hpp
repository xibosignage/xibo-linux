#pragma once

#include <utility>
#include "PlayerError.hpp"

template<typename Result>
using ResponseResult = std::pair<PlayerError, Result>;
