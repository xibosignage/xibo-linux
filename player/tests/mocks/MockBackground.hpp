#pragma once

#include "control/IBackground.hpp"
#include "constants.hpp"

#include <gmock/gmock.h>
#include <optional>

class MockBackground : public IBackground
{
public:
    MockBackground(std::unique_ptr<IImageAdaptor>&& handler) :
        m_handler(std::move(handler))
    {
    }

    IImageAdaptor& handler()
    {
        return *m_handler;
    }

    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD0(show, void());

private:
    std::unique_ptr<IImageAdaptor> m_handler;
};


class MockOneColorBackground : public MockBackground
{
public:
    MockOneColorBackground(uint32_t color, std::unique_ptr<IImageAdaptor>&& handler) :
        MockBackground(std::move(handler)), m_color(color)
    {
    }

    uint32_t color() const
    {
        return m_color;
    }

private:
    uint32_t m_color;
};


class MockImageBackground : public MockBackground
{
public:
    MockImageBackground(const FilePath& path, std::unique_ptr<IImageAdaptor>&& handler) :
        MockBackground(std::move(handler)), m_path(path)
    {
    }

    FilePath path() const
    {
        return m_path;
    }

private:
    FilePath m_path;
};
