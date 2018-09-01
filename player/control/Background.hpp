#pragma once

#include "IBackground.hpp"

#include <memory>

class Background : public IBackground
{
public:
    static std::shared_ptr<IBackground> create_one_color(const std::string& hex_color,
                                                         int width,
                                                         int height);
    static std::shared_ptr<IBackground> create_with_image(const std::string& image_path,
                                                          int width,
                                                          int height);

    Background(const Background&) = delete;
    Background& operator=(const Background&) = delete;

    int width() const override;
    int height() const override;
    void set_size(int width, int height) override;

    uint32_t hex_color_number() const override;
    const std::string& hex_color() const override;
    void set_color(const std::string& hex_color) override;

    void set_image(const std::string& image_path) override;
    void show() override;
    IImageWrapper& handler() override;

private:
    Background(int width, int height, std::shared_ptr<IImageWrapper> handler);
    uint32_t color_to_hex_number(const std::string& hex_color) const;

private:
    std::shared_ptr<IImageWrapper> m_handler;
    std::string m_hex_color;

};
