#pragma once

#include "IBackground.hpp"

#include <memory>

class Background : public IBackground
{
public:
    static std::shared_ptr<IBackground> createOneColor(const std::string& hexColor,
                                                       int width,
                                                       int height);
    static std::shared_ptr<IBackground> createWithImage(const std::string& imagePath,
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
    IImageWrapper& handler() override;

private:
    Background(std::shared_ptr<IImageWrapper> handler);
    uint32_t colorToHexNumber(const std::string& hexColor) const;

private:
    std::shared_ptr<IImageWrapper> m_handler;
    std::string m_hexColor;

};
