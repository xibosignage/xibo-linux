#pragma once

#include <vector>
#include <memory>

class IRegion;
class IRegionView;
class IMedia;

struct ParsedMedia;
struct ParsedRegion;
struct RegionOptions;
struct MediaOptions;

class RegionBuilder
{
public:
    std::unique_ptr<IRegion> build(const ParsedRegion& region);

private:
    std::shared_ptr<IRegionView> createView(const RegionOptions& options);

    void addMedia(IRegion& region, const std::vector<ParsedMedia>& media);
    void addTransitionsToMedia(const MediaOptions& options, IMedia& media, IRegion& region);
    std::pair<int, int> getMediaPosInRegion(IRegion& region, IMedia& media);

};
