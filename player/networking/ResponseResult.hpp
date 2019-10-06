#pragma once

#include "common/PlayerError.hpp"

#include <utility>

template <typename Result>
using ResponseResult = std::pair<PlayerError, Result>;
