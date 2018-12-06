#pragma once

#include <memory>
#include <vector>
#include <boost/optional/optional_fwd.hpp>

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

    RegionBuilder& id(int id);
    RegionBuilder& width(int width);
    RegionBuilder& height(int height);
    RegionBuilder& zorder(const boost::optional<int>& zorder);
    RegionBuilder& loop(const boost::optional<bool>& loop);
    RegionBuilder& content(std::vector<ContentWithPos>&& content);

protected:
    virtual std::unique_ptr<IFixedLayoutAdaptor> createAdaptor();

private:
    std::unique_ptr<IRegion> createRegion();

    void loopContent(IRegion& region);
    void addAllContent(IRegion& region);
    void checkWidth(int width);
    void checkHeight(int height);

private:
    int m_id;
    int m_width;
    int m_height;
    int m_zorder;
    bool m_loop;
    std::vector<ContentWithPos> m_content;

};
