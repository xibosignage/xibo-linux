#pragma once

#include "AbstractBuilder.hpp"

#include "control/IBackground.hpp"
#include "parsers/BackgroundOptions.hpp"
#include "adaptors/GtkImageAdaptor.hpp"

#include "utils/IFileSystemAdaptor.hpp"
#include "utils/FilePath.hpp"

class BackgroundBuilder;

template<>
struct BuilderTraits<BackgroundBuilder>
{
    using Component = IBackground;
    using DefaultHandler = GtkImageAdaptor;
    using Options = ResourcesXlf::BackgroundOptions;
};

class BackgroundBuilder : public AbstractBuilder<BackgroundBuilder>
{
public:
    BackgroundBuilder();

    BackgroundBuilder& filesystem(std::unique_ptr<IFileSystemAdaptor>&& filesystem);

protected:
    BackgroundBuilder& retrieveOptions(const ResourcesXlf::BackgroundOptions& opts) override;
    std::unique_ptr<IBackground> create() override;

private:
    std::unique_ptr<IBackground> createBackground(uint32_t color);
    std::unique_ptr<IBackground> createBackground(const FilePath& path);

    int getWidthOption(int width);
    int getHeightOption(int height);
    uint32_t getColorOption(const boost::optional<std::string>& colorOpt);
    FilePath getPathOption(const boost::optional<std::string>& pathOpt);

    void checkWidth(int width);
    void checkHeight(int height);
    void checkPath(const FilePath& pathOpt);

private:
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    int m_width;
    int m_height;
    uint32_t m_hexColor;
    FilePath m_path;

};
