#pragma once

#include <memory>
#include <boost/optional/optional.hpp>

#include "utils/IFileSystemAdaptor.hpp"
#include "utils/FilePath.hpp"
#include "parsers/BackgroundOptions.hpp"

class IBackground;
class IImageAdaptor;

class BackgroundBuilder
{
public:
    BackgroundBuilder();
    std::unique_ptr<IBackground> build();

    BackgroundBuilder& options(const ResourcesXlf::BackgroundOptions& opts);
    BackgroundBuilder& createAdaptor(std::unique_ptr<IImageAdaptor>&& createAdaptor);
    BackgroundBuilder& filesystem(std::unique_ptr<IFileSystemAdaptor>&& filesystem);

private:
    std::unique_ptr<IBackground> createBackground(uint32_t color);
    std::unique_ptr<IBackground> createBackground(const FilePath& path);
    std::unique_ptr<IImageAdaptor> createAdaptor();

    int getWidthOption(int width);
    int getHeightOption(int height);
    uint32_t getColorOption(const boost::optional<std::string>& colorOpt);
    FilePath getPathOption(const boost::optional<std::string>& pathOpt);

    void checkWidth(int width);
    void checkHeight(int height);
    void checkPath(const FilePath& pathOpt);

private:
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    std::unique_ptr<IImageAdaptor> m_adaptor;
    int m_width;
    int m_height;
    uint32_t m_hexColor;
    FilePath m_path;

};
