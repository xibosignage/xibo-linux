#pragma once

#include <gmock/gmock.h>
#include <memory>

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

const int NEW_DEFAULT_WIDTH = 200;
const int NEW_DEFAULT_HEIGHT = 200;

template<typename Testee, typename MockAdaptor, typename... Args>
auto construct(Args... args)
{
    auto adaptor = std::make_unique<testing::NiceMock<MockAdaptor>>();
    auto adaptorRaw = adaptor.get();
    auto testee = std::make_shared<Testee>(std::forward<Args>(args)..., std::move(adaptor));
    return std::pair{testee, adaptorRaw};
}

template<typename T>
std::unique_ptr<T> unique(T* pointer)
{
    return std::unique_ptr<T>{pointer};
}
