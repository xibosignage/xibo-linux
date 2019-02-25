#include "BackgroundBuilder.hpp"

#include "control/Background.hpp"
#include "adaptors/GtkImageAdaptor.hpp"

#include "utils/Resources.hpp"
#include "utils/ColorToHexConverter.hpp"
#include "utils/FileSystemAdaptor.hpp"

BackgroundBuilder::BackgroundBuilder()
{
    m_filesystem = std::make_unique<FileSystemAdaptor>();
}

BackgroundBuilder& BackgroundBuilder::filesystem(std::unique_ptr<IFileSystemAdaptor>&& filesystem)
{
    m_filesystem = std::move(filesystem);
    return *this;
}

void BackgroundBuilder::retrieveOptions(const BackgroundOptions& opts)
{
    m_width = getWidthOption(opts.width());
    m_height = getHeightOption(opts.height());
    m_uri = getUriOption(opts.path());
    m_hexColor = getColorOption(opts.color());
}

std::unique_ptr<IBackground> BackgroundBuilder::create()
{
    if(!m_uri.isValid())
        return createBackground(m_hexColor);
    else
        return createBackground(m_uri);
}

std::unique_ptr<IImageAdaptor> BackgroundBuilder::createDefaultHandler()
{
    return std::make_unique<GtkImageAdaptor>();
}

std::unique_ptr<IBackground> BackgroundBuilder::createBackground(uint32_t color)
{
    return std::unique_ptr<OneColorBackground>(new OneColorBackground{m_width, m_height, color, createHandler()});
}

std::unique_ptr<IBackground> BackgroundBuilder::createBackground(const Uri& uri)
{
    return std::unique_ptr<ImageBackground>(new ImageBackground{m_width, m_height, uri, createHandler()});
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

Uri BackgroundBuilder::getUriOption(const boost::optional<std::string>& pathOpt)
{
    if(pathOpt)
    {
        auto fullPath = Resources::directory() / pathOpt.value();

        checkPath(fullPath);

        return Uri{Uri::Scheme::File, fullPath};
    }
    return {};
}

void BackgroundBuilder::checkPath(const FilePath& path)
{
    if(!m_filesystem->isRegularFile(path))
        throw std::runtime_error("Not valid path");
}
