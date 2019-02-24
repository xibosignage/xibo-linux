#pragma once

#include <gmock/gmock.h>
#include <memory>

#include "test_constants.hpp"

template<int MinWidth, int MinHeight>
const std::vector<Size> invalidSizes = {
    {-1, DEFAULT_HEIGHT},
    {DEFAULT_WIDTH, -1},
    {MinWidth - 1, DEFAULT_HEIGHT},
    {DEFAULT_WIDTH, MinHeight - 1},
    {MinWidth - 1, MinHeight - 1},
    {std::numeric_limits<int>::min(), DEFAULT_HEIGHT},
    {DEFAULT_WIDTH, std::numeric_limits<int>::min()}
};

template<typename Testee, typename... Args>
auto construct(Args&&... args)
{
    return std::make_unique<Testee>(std::forward<Args>(args)...);
}

template<typename Testee, typename... Args>
auto constructMock(Args&&... args)
{
    return std::make_unique<testing::NiceMock<Testee>>(std::forward<Args>(args)...);
}

template<typename T>
std::unique_ptr<T> unique(T* pointer)
{
    return std::unique_ptr<T>{pointer};
}
