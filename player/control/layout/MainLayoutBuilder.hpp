#pragma once

#include <vector>
#include <memory>

class IMainLayout;
class IMainLayoutView;
class IImage;

struct LayoutOptions;
struct ParsedLayout;
struct ParsedRegion;

class MainLayoutBuilder
{
public:
    std::unique_ptr<IMainLayout> build(const ParsedLayout& layout);

private:
    std::shared_ptr<IMainLayoutView> createView(const LayoutOptions& options);
    std::shared_ptr<IImage> createBackground(const LayoutOptions& options);
    void addRegions(IMainLayout& layout, const std::vector<ParsedRegion>& regions);

};
