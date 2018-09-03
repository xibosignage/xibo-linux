#pragma once

#include "IBackground.hpp"

#include <memory>

class Background : public IBackground
{
public:
    static std::unique_ptr<IBackground> createOneColor(const std::string& hexColor,
                                                       int width,
                                                       int height);
    static std::unique_ptr<IBackground> createWithImage(const std::string& imagePath,
                                                        int width,
                                                        int height);

    Background(const Background&) = delete;
    Background& operator=(const Background&) = delete;

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;

    uint32_t hexColorNumber() const override;
    const std::string& hexColor() const override;
    void setColor(const std::string& hexColor) override;

    void setImage(const std::string& imagePath) override;
    void show() override;
    IImageAdaptor& handler() override;

private:
    Background(std::unique_ptr<IImageAdaptor> handler);
    uint32_t colorToHexNumber(const std::string& hexColor) const;

private:
    std::unique_ptr<IImageAdaptor> m_handler;
    std::string m_hexColor;

};
