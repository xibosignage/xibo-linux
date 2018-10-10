#pragma once

#include "IBackground.hpp"

#include <memory>

class Background : public IBackground
{
public:
    Background(std::unique_ptr<IImageAdaptor>&& handler);

    Background(const Background&) = delete;
    Background& operator=(const Background&) = delete;

    int width() const override;
    int height() const override;
    void scale(double scaleX, double scaleY) override;

    void show() override;
    IImageAdaptor& handler() override;

private:
    std::unique_ptr<IImageAdaptor> m_handler;

};
