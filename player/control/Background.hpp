#pragma once

#include "IBackground.hpp"

#include <memory>

class Background : public IBackground
{
public:
    Background(int width, int height);
    Background(int width, int height, std::unique_ptr<IImageAdaptor> handler);

    Background(const Background&) = delete;
    Background& operator=(const Background&) = delete;

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;

    void setColor(const std::string& hexColor) override;
    void setImage(const uint8_t* imageData, size_t dataSize) override;

    void show() override;
    IImageAdaptor& handler() override;

private:
    uint32_t colorToHexNumber(const std::string& hexColor) const;
    bool isValidColor(const std::string& hexColor) const;

private:
    std::unique_ptr<IImageAdaptor> m_handler;

};
