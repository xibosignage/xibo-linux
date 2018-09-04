#include "BackgroundFactory.hpp"

#include "control/Background.hpp"
#include "adaptors/IImageAdaptor.hpp"

#include <fstream>

std::unique_ptr<IBackground> BackgroundFactory::create(int width, int height,
                                                       const std::filesystem::path& imagePath,
                                                       const std::string& color)
{
    if(!color.empty() && imagePath.empty())
        return createOneColor(width, height, color);

    if(!imagePath.empty())
        return createWithImage(width, height, imagePath);

    return nullptr;
}

std::unique_ptr<IBackground> BackgroundFactory::createWithImage(int width, int height,
                                                                const std::filesystem::path& imagePath)
{
    auto background = std::make_unique<Background>(width, height);
    auto imageData = getRawData(imagePath);
    background->setImage(imageData.data(), imageData.size());
    return background;
}

std::unique_ptr<IBackground> BackgroundFactory::createOneColor(int width, int height,
                                                               const std::string& color)
{
    auto background = std::make_unique<Background>(width, height);
    background->setColor(color);
    return background;
}

std::vector<uint8_t> BackgroundFactory::getRawData(const std::filesystem::path& imagePath)
{
    if(!std::filesystem::exists(imagePath))
        throw std::runtime_error("Background path doesn't exist");

    std::ifstream ifs(imagePath.string(), std::ios::binary | std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<uint8_t> result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(reinterpret_cast<char*>(&result[0]), pos);

    return result;
}
