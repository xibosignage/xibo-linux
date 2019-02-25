#pragma once

#include "AbstractBuilder.hpp"

#include "control/IBackground.hpp"
#include "options/BackgroundOptions.hpp"
#include "adaptors/IImageAdaptor.hpp"

#include "utils/IFileSystemAdaptor.hpp"
#include "utils/Uri.hpp"

class BackgroundBuilder;

template<>
struct BuilderTraits<BackgroundBuilder>
{
    using Component = IBackground;
    using Handler = IImageAdaptor;
    using Options = BackgroundOptions;
};

const std::string DEFAULT_COLOR = "#000";

class BackgroundBuilder : public AbstractBuilder<BackgroundBuilder>
{
public:
    BackgroundBuilder();

    BackgroundBuilder& filesystem(std::unique_ptr<IFileSystemAdaptor>&& filesystem);

protected:
    void retrieveOptions(const BackgroundOptions& opts) override;
    std::unique_ptr<IBackground> create() override;
    std::unique_ptr<IImageAdaptor> createDefaultHandler() override;

private:
    std::unique_ptr<IBackground> createBackground(uint32_t color);
    std::unique_ptr<IBackground> createBackground(const Uri& uri);

    int getWidthOption(int width);
    int getHeightOption(int height);
    uint32_t getColorOption(const boost::optional<std::string>& colorOpt);
    Uri getUriOption(const boost::optional<std::string>& pathOpt);

    void checkWidth(int width);
    void checkHeight(int height);
    void checkPath(const FilePath& pathOpt);

private:
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    int m_width;
    int m_height;
    uint32_t m_hexColor;
    Uri m_uri;

};
