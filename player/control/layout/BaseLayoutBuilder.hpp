#pragma once

#include <vector>
#include <memory>

class IMainLayout;
class IOverlayLayout;
class IImage;

struct LayoutOptions;
struct ParsedLayout;
struct ParsedRegion;

class BaseLayoutBuilder
{
public:
    std::unique_ptr<IMainLayout> build(const ParsedLayout& layout);

    virtual ~BaseLayoutBuilder() = default;

protected:
    virtual std::shared_ptr<IOverlayLayout> createView(const LayoutOptions& options) = 0;

private:
    void addRegions(IMainLayout& layout, const std::vector<ParsedRegion>& regions);

};
