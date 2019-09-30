#pragma once

#include "constants.hpp"
#include "control/layout/MainLayout.hpp"
#include "control/layout/MainLayoutOptions.hpp"
#include "control/widgets/Image.hpp"

class FilePath;

class MainLayoutParser
{
public:
    virtual ~MainLayoutParser() = default;

    struct Error : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    std::unique_ptr<Xibo::MainLayout> parseBy(int layoutId);

protected:
    std::unique_ptr<Xibo::MainLayout> layoutFrom(const PtreeNode& node);
    MainLayoutOptions optionsFrom(const PtreeNode& node);
    Uri backgroundUriFrom(const PtreeNode& node);
    Color backgroundColorFrom(const PtreeNode& node);

    virtual std::shared_ptr<Xibo::Image> createBackground(const MainLayoutOptions& options);
    void addRegions(Xibo::MainLayout& layout, const PtreeNode& node);
};
