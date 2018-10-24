#pragma once

#include "test_utils.hpp"

#include "control/Background.hpp"
#include "mocks/MockImageAdaptor.hpp"

const uint32_t DEFAULT_COLOR = 255;


template<typename T>
auto constructBackground(std::unique_ptr<MockImageAdaptor>&& adaptor);

template<>
auto constructBackground<OneColorBackground>(std::unique_ptr<MockImageAdaptor>&& adaptor)
{
    return construct<OneColorBackground>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_COLOR, std::move(adaptor));
}

template<>
auto constructBackground<ImageBackground>(std::unique_ptr<MockImageAdaptor>&& adaptor)
{
    return construct<ImageBackground>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, std::move(adaptor));
}

template<typename T>
class BackgroundTest : public testing::Test
{
public:
    auto constructBackground()
    {
        if constexpr(std::is_same_v<T, OneColorBackground>)
        {
            return construct<OneColorBackground>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_COLOR, unique(m_adaptor));
        }
        else
        {
            return construct<ImageBackground>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, unique(m_adaptor));
        }
    }

protected:
    void SetUp() override
    {
        m_adaptor = new testing::NiceMock<MockImageAdaptor>;
    }

    void TearDown() override
    {
        m_adaptor = nullptr;
    }

    MockImageAdaptor& adaptor()
    {
        return *m_adaptor;
    }

private:
    MockImageAdaptor* m_adaptor = nullptr;

};
