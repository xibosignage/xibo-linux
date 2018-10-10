#include "BackgroundBuilder.hpp"

#include "control/Background.hpp"
#include "adaptors/IImageAdaptor.hpp"

#include "utils/utilities.hpp"
#include "utils/Helpers.hpp"
#include "utils/ColorToHexConverter.hpp"

BackgroundBuilder::BackgroundBuilder(std::unique_ptr<IFileSystemAdaptor>&& filesystem) :
    m_filesystem(std::move(filesystem))
{
}

std::unique_ptr<IBackground> BackgroundBuilder::build()
{
    assert(m_adaptor);

    m_adaptor->setSize(m_width, m_height);

    if(m_hexColor && m_path.empty())
        m_adaptor->setColor(m_hexColor.value());

    if(!m_path.empty())
        m_adaptor->setImage(m_path.string());

    return createBackground();
}

std::unique_ptr<IBackground> BackgroundBuilder::createBackground()
{
    return std::make_unique<Background>(std::move(m_adaptor));
}

BackgroundBuilder& BackgroundBuilder::adaptor(std::unique_ptr<IImageAdaptor>&& adaptor)
{
    m_adaptor = std::move(adaptor);
    return *this;
}

BackgroundBuilder& BackgroundBuilder::width(int width)
{
    checkWidth(width);

    m_width = width;
    return *this;
}

void BackgroundBuilder::checkWidth(int width)
{
    if(width < MIN_DISPLAY_WIDTH || width > MAX_DISPLAY_WIDTH)
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
    if(height < MIN_DISPLAY_HEIGHT || height > MAX_DISPLAY_HEIGHT)
        throw std::invalid_argument("Height is too small/large");
}

// TODO specify always provided or not
BackgroundBuilder& BackgroundBuilder::color(const boost::optional<std::string>& color)
{
    if(color)
    {
        ColorToHexConverter converter;
        m_hexColor = converter.colorToHex(color.value());
    }
    return *this;
}

BackgroundBuilder& BackgroundBuilder::path(const boost::optional<std::string>& path)
{
    auto fullPath = m_filesystem->resourcesDirectory() / path.value_or(std::string{});

    checkPath(fullPath);

    m_path = fullPath;
    return *this;
}

void BackgroundBuilder::checkPath(FilePath path)
{
    if(!m_filesystem->isRegularFile(path))
        throw std::runtime_error("Not valid path");
}
