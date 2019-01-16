#include "BackgroundBuilder.hpp"

#include "control/Background.hpp"
#include "adaptors/GtkImageAdaptor.hpp"

#include "utils/Resources.hpp"
#include "utils/ColorToHexConverter.hpp"
#include "utils/FileSystemAdaptor.hpp"
#include "constants.hpp"

const std::string DEFAULT_COLOR = "#000";

BackgroundBuilder::BackgroundBuilder()
{
    m_filesystem = std::make_unique<FileSystemAdaptor>();
}

std::unique_ptr<IBackground> BackgroundBuilder::build()
{
    if(m_hexColor && m_path.empty())
        return createBackground(m_hexColor.value());

    if(!m_path.empty())
        return createBackground(m_path);

    throw std::runtime_error("Background can't be created");
}

std::unique_ptr<IBackground> BackgroundBuilder::createBackground(uint32_t color)
{
    return std::make_unique<OneColorBackground>(m_width, m_height, color, createAdaptor());
}

std::unique_ptr<IBackground> BackgroundBuilder::createBackground(const FilePath& path)
{
    return std::make_unique<ImageBackground>(m_width, m_height, path, createAdaptor());
}

std::unique_ptr<IImageAdaptor> BackgroundBuilder::createAdaptor()
{
    return std::make_unique<GtkImageAdaptor>();
}

IFileSystemAdaptor& BackgroundBuilder::filesystem()
{
    return *m_filesystem;
}

BackgroundBuilder& BackgroundBuilder::width(int width)
{
    checkWidth(width);

    m_width = width;
    return *this;
}

void BackgroundBuilder::checkWidth(int width)
{
    if(width < MIN_DISPLAY_WIDTH)
        throw std::invalid_argument("Width is too small/large");
}

BackgroundBuilder& BackgroundBuilder::height(int height)
{
    checkHeight(height);

    m_height = height;
    return *this;
}

void BackgroundBuilder::checkHeight(int height)
{
    if(height < MIN_DISPLAY_HEIGHT)
        throw std::invalid_argument("Height is too small/large");
}

BackgroundBuilder& BackgroundBuilder::color(const boost::optional<std::string>& color)
{
    ColorToHexConverter converter;
    m_hexColor = converter.colorToHex(color.value_or(DEFAULT_COLOR));
    return *this;
}

BackgroundBuilder& BackgroundBuilder::path(const boost::optional<std::string>& path)
{
    if(path)
    {
        auto fullPath = Resources::directory() / path.value();

        checkPath(fullPath);

        m_path = fullPath;
    }
    return *this;
}

void BackgroundBuilder::checkPath(FilePath path)
{
    if(!filesystem().isRegularFile(path))
        throw std::runtime_error("Not valid path");
}
