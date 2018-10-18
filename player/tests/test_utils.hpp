#pragma once

#include <gmock/gmock.h>
#include <memory>

#include "constants.hpp"
#include "utils/Helpers.hpp"

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

const double DEFAULT_XSCALE = 0.5;
const double DEFAULT_YSCALE = 0.5;

const int NEW_DEFAULT_WIDTH = 200;
const int NEW_DEFAULT_HEIGHT = 200;

template<int MaxWidth, int MinWidth, int MaxHeight, int MinHeight>
const std::vector<Size> invalidSizes = {
    {-1, DEFAULT_HEIGHT},
    {DEFAULT_WIDTH, -1},
    {MaxWidth + 1, DEFAULT_HEIGHT},
    {DEFAULT_WIDTH, MaxHeight + 1},
    {MinWidth - 1, DEFAULT_HEIGHT},
    {DEFAULT_WIDTH, MinHeight - 1},
    {MinWidth - 1, MaxHeight + 1},
    {MaxWidth + 1, MinHeight - 1},
    {MaxWidth + 1, MaxHeight + 1},
    {MinWidth - 1, MinHeight - 1},
    {std::numeric_limits<int>::max(), DEFAULT_HEIGHT},
    {DEFAULT_WIDTH, std::numeric_limits<int>::max()},
    {std::numeric_limits<int>::min(), DEFAULT_HEIGHT},
    {DEFAULT_WIDTH, std::numeric_limits<int>::min()}
};

template<int MaxX, int MinX, int MaxY, int MinY>
const std::vector<Point> invalidPositions = {
    {MaxX + 1, DEFAULT_YPOS},
    {DEFAULT_XPOS, MaxY + 1},
    {MinX - 1, DEFAULT_YPOS},
    {DEFAULT_XPOS, MinY - 1},
    {MinX - 1, MaxY + 1},
    {MaxX + 1, MinY - 1},
    {MaxX + 1, MaxY + 1},
    {MinX - 1, MinY - 1},
    {std::numeric_limits<int>::max(), DEFAULT_YPOS},
    {DEFAULT_XPOS, std::numeric_limits<int>::max()},
    {std::numeric_limits<int>::min(), DEFAULT_YPOS},
    {DEFAULT_XPOS, std::numeric_limits<int>::min()}
};

template<typename Testee, typename MockAdaptor, typename... Args>
auto construct(Args... args)
{
    auto adaptor = std::make_unique<testing::NiceMock<MockAdaptor>>();
    auto adaptorRaw = adaptor.get();
    auto testee = std::make_shared<Testee>(std::forward<Args>(args)..., std::move(adaptor));
    return std::pair{testee, adaptorRaw};
}

template<typename Testee, typename MockAdaptor, typename... Args>
auto fake_construct(Args... args)
{
    return std::make_unique<testing::NiceMock<Testee>>(std::forward<Args>(args)..., std::make_unique<testing::NiceMock<MockAdaptor>>());
}

template<typename T>
std::unique_ptr<T> unique(T* pointer)
{
    return std::unique_ptr<T>{pointer};
}
