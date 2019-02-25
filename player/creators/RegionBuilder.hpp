#pragma once

#include "AbstractBuilder.hpp"

#include "control/IRegion.hpp"
#include "options/RegionOptions.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"

#include <vector>
#include <boost/optional/optional_fwd.hpp>

class IRegionContent;
class ITimerProvider;
class RegionBuilder;

struct ContentWithPos
{
    std::unique_ptr<IRegionContent> content;
    int x;
    int y;
};

template<>
struct BuilderTraits<RegionBuilder>
{
    using Component = IRegion;
    using Handler = IFixedLayoutAdaptor;
    using Options = RegionOptions;
};

const int DEFAULT_REGION_ZORDER = 0;
const RegionOptions::Loop DEFAULT_REGION_LOOP = RegionOptions::Loop::Disable;

class RegionBuilder : public AbstractBuilder<RegionBuilder>
{
public:
    RegionBuilder& content(std::vector<ContentWithPos>&& content);

protected:
    void retrieveOptions(const RegionOptions& opts) override;
    std::unique_ptr<IRegion> create() override;
    std::unique_ptr<IFixedLayoutAdaptor> createDefaultHandler() override;
    void doSetup(IRegion& region) override;

private:
    int getIdOption(int id);
    int getWidthOption(int width);
    int getHeightOption(int height);
    int getZorderOption(const boost::optional<int>& zorderOpt);
    RegionOptions::Loop getLoopOption(const boost::optional<RegionOptions::Loop>& loopOpt);

    void loopContent(IRegion& region);
    void addAllContent(IRegion& region);
    void checkWidth(int width);
    void checkHeight(int height);

private:
    int m_id;
    int m_width;
    int m_height;
    int m_zorder;
    RegionOptions::Loop m_loop;
    std::vector<ContentWithPos> m_content;

};
