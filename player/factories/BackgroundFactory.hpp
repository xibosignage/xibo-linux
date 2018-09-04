#pragma once

#include <memory>
#include <filesystem>

class IBackground;

class BackgroundFactory
{
public:
    BackgroundFactory() = default;

    std::unique_ptr<IBackground> create(int width, int height,
                                        const std::filesystem::path& imagePath,
                                        const std::string& color);

private:
    std::unique_ptr<IBackground> createWithImage(int width, int height,
                                                 const std::filesystem::path& imagePath);
    std::unique_ptr<IBackground> createOneColor(int width, int height,
                                                const std::string& color);
    std::vector<uint8_t> getRawData(const std::filesystem::path& imagePath);

};
