#pragma once

#include <memory>
#include <boost/optional/optional.hpp>

#include "control/IBackground.hpp"
#include "adaptors/IImageAdaptor.hpp"
#include "utils/IFileSystemAdaptor.hpp"

class BackgroundBuilder
{
public:
    BackgroundBuilder();
    std::unique_ptr<IBackground> build();

    BackgroundBuilder& width(int width);
    BackgroundBuilder& height(int height);
    BackgroundBuilder& color(const boost::optional<std::string>& color);
    BackgroundBuilder& path(const boost::optional<std::string>& path);

protected:
    virtual std::unique_ptr<IImageAdaptor> createAdaptor();
    virtual IFileSystemAdaptor& filesystem();

private:
    std::unique_ptr<IBackground> createBackground(uint32_t color);
    std::unique_ptr<IBackground> createBackground(const FilePath& path);

    void checkWidth(int width);
    void checkHeight(int height);
    void checkPath(FilePath path);

private:
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    int m_width;
    int m_height;
    boost::optional<uint32_t> m_hexColor;
    FilePath m_path;

};
