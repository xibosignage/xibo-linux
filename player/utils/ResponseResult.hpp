#pragma once

#include <boost/system/error_code.hpp>
#include <utility>

template<typename Result>
using ResponseResult = std::pair<boost::system::error_code, Result>;
