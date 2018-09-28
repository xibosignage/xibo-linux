#pragma once

#include <memory>
#include <boost/optional/optional_fwd.hpp>
#include <filesystem>

#include "control/IBackground.hpp"

class BackgroundBuilder
{
public:
    template<typename BackgroundFactory>
    std::unique_ptr<IBackground> build()
    {
        auto background = BackgroundFactory().create();

        prepareBackground(*background);

        return background;
    }

    BackgroundBuilder& setWidth(int width);
    BackgroundBuilder& setHeight(int height);
    BackgroundBuilder& setPath(const boost::optional<std::string>& path);
    BackgroundBuilder& setColor(const boost::optional<std::string>& color);

private:
    void prepareBackground(IBackground& background);

private:
    int m_width;
    int m_height;
    std::string m_color;
    std::filesystem::path m_path;

};
