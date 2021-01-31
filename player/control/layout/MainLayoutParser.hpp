#pragma once

#include "common/parsing/Parsing.hpp"
#include "control/layout/MainLayout.hpp"
#include "control/layout/MainLayoutOptions.hpp"
#include "control/widgets/Image.hpp"

class FilePath;

class MainLayoutParser
{
public:
    MainLayoutParser(bool globalStatsEnabled);
    virtual ~MainLayoutParser() = default;

    struct Error : PlayerRuntimeError
    {
        Error(const std::string& domain, int layoutId, const std::string& reason);
    };

    std::unique_ptr<Xibo::MainLayout> parseBy(int layoutId);

protected:
    std::unique_ptr<Xibo::MainLayout> layoutFrom(const XmlNode& node);
    MainLayoutOptions optionsFrom(const XmlNode& node);
    boost::optional<Uri> backgroundUriFrom(const XmlNode& node);
    Color backgroundColorFrom(const XmlNode& node);

    virtual std::shared_ptr<Xibo::Image> createBackground(const MainLayoutOptions& options);
    void addRegions(Xibo::MainLayout& layout, const XmlNode& node);

private:
    bool globalStatsEnabled_;
    int layoutId_;
};
