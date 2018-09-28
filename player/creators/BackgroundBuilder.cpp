#include "BackgroundBuilder.hpp"
#include "utils/utilities.hpp"

#include <boost/optional/optional.hpp>

BackgroundBuilder& BackgroundBuilder::setWidth(int width)
{
    m_width = width;
    return *this;
}

BackgroundBuilder& BackgroundBuilder::setHeight(int height)
{
    m_height = height;
    return *this;
}

BackgroundBuilder& BackgroundBuilder::setColor(const boost::optional<std::string>& color)
{
    m_color = color.value_or(std::string{});
    return *this;
}

BackgroundBuilder& BackgroundBuilder::setPath(const boost::optional<std::string>& path)
{
    m_path = utils::resourcesDir() / path.value_or(std::string{});
    return *this;
}

void BackgroundBuilder::prepareBackground(IBackground& background)
{
    background.setSize(m_width, m_height);

    if(!m_color.empty() && !std::filesystem::is_regular_file(m_path))
        background.setColor(m_color);

    if(!m_path.empty())
    {
        auto imageData = utils::getRawData(m_path);
        background.setImage(imageData.data(), imageData.size());
    }
}
