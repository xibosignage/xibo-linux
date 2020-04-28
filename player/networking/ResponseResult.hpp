#pragma once

#include "common/PlayerError.hpp"

#include <boost/thread/future.hpp>
#include <utility>

template <typename Result>
using ResponseResult = std::pair<PlayerError, Result>;

template <typename Result>
using FutureResponseResult = boost::future<ResponseResult<Result>>;
