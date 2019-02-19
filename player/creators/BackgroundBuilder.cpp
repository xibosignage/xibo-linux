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
    if(m_path.empty())
        return createBackground(m_hexColor);
    else
        return createBackground(m_path);
}

BackgroundBuilder& BackgroundBuilder::options(const ResourcesXlf::BackgroundOptions& opts)
{
    m_width = getWidthOption(opts.width());
    m_height = getHeightOption(opts.height());
    m_path = getPathOption(opts.path());
    m_hexColor = getColorOption(opts.color());
    return *this;
}

BackgroundBuilder&BackgroundBuilder::createAdaptor(std::unique_ptr<IImageAdaptor>&& adaptor)
{
    m_adaptor = std::move(adaptor);
    return *this;
}

BackgroundBuilder& BackgroundBuilder::filesystem(std::unique_ptr<IFileSystemAdaptor>&& filesystem)
{
    m_filesystem = std::move(filesystem);
    return *this;
}

std::unique_ptr<IBackground> BackgroundBuilder::createBackground(uint32_t color)
{
    return std::unique_ptr<OneColorBackground>(new OneColorBackground{m_width, m_height, color, createAdaptor()});
}

std::unique_ptr<IBackground> BackgroundBuilder::createBackground(const FilePath& path)
{
    return std::unique_ptr<ImageBackground>(new ImageBackground{m_width, m_height, path, createAdaptor()});
}

std::unique_ptr<IImageAdaptor> BackgroundBuilder::createAdaptor()
{
    if(!m_adaptor) return std::make_unique<GtkImageAdaptor>();

    return std::move(m_adaptor);
}

int BackgroundBuilder::getWidthOption(int width)
{
    checkWidth(width);

    return width;
}

void BackgroundBuilder::checkWidth(int width)
{
    if(width < MIN_DISPLAY_WIDTH)
        throw std::invalid_argument("Width is too small/large");
}

int BackgroundBuilder::getHeightOption(int height)
{
    checkHeight(height);

    return height;
}

void BackgroundBuilder::checkHeight(int height)
{
    if(height < MIN_DISPLAY_HEIGHT)
        throw std::invalid_argument("Height is too small/large");
}

uint32_t BackgroundBuilder::getColorOption(const boost::optional<std::string>& colorOpt)
{
    ColorToHexConverter converter;
    return converter.colorToHex(colorOpt.value_or(DEFAULT_COLOR));
}

FilePath BackgroundBuilder::getPathOption(const boost::optional<std::string>& pathOpt)
{
    if(pathOpt)
    {
        auto fullPath = Resources::directory() / pathOpt.value();

        checkPath(fullPath);

        return fullPath;
    }
    return {};
}

void BackgroundBuilder::checkPath(const FilePath& path)
{
    if(!m_filesystem->isRegularFile(path))
        throw std::runtime_error("Not valid path");
}
