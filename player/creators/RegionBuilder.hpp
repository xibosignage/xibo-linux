#pragma once

#include <memory>
#include <vector>
#include <boost/optional/optional_fwd.hpp>

#include "parsers/RegionOptions.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"

class IRegion;
class IRegionContent;
class IFixedLayoutAdaptor;
class ITimerProvider;

struct ContentWithPos
{
    std::unique_ptr<IRegionContent> content;
    int x;
    int y;
};

const int DEFAULT_ZORDER = 0;
const bool DEFAULT_LOOP = false;

class RegionBuilder
{
public:
    std::unique_ptr<IRegion> build();

    RegionBuilder& options(const ResourcesXlf::RegionOptions& opts);
    RegionBuilder& content(std::vector<ContentWithPos>&& content);
    RegionBuilder& adaptor(std::unique_ptr<IFixedLayoutAdaptor>&& adaptor);

private:
    std::unique_ptr<IRegion> createRegion();
    std::unique_ptr<IFixedLayoutAdaptor> createAdaptor();

    int getIdOption(int id);
    int getWidthOption(int width);
    int getHeightOption(int height);
    int getZorderOption(const boost::optional<int>& zorderOpt);
    bool getLoopOption(const boost::optional<bool>& loopOpt);

    void loopContent(IRegion& region);
    void addAllContent(IRegion& region);
    void checkWidth(int width);
    void checkHeight(int height);

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_adaptor;
    int m_id;
    int m_width;
    int m_height;
    int m_zorder;
    bool m_loop;
    std::vector<ContentWithPos> m_content;

};
