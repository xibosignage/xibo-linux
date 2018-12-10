#pragma once

#include "IRegion.hpp"

#include <vector>
#include <boost/noncopyable.hpp>

class IRegionContent;
class ITimerProvider;

class Region : public IRegion, private boost::noncopyable
{
public:
    Region(int id, int width, int height, int zorder, std::unique_ptr<IFixedLayoutAdaptor>&& handler);
    ~Region() override;

    int width() const override;
    int height() const override;
    void scale(double scaleX, double scaleY) override;

    void loopContent() override;
    bool contentLooped() const override;
    int id() const override;
    int zorder() const override;
    void show() override;

    void addContent(std::unique_ptr<IRegionContent>&& content, int x, int y) override;
    IFixedLayoutAdaptor& handler() override;

private:
    void scaleContent(double scaleX, double scaleY);

    void placeContent(size_t mediaIndex);
    void removeContent(size_t mediaIndex);
    void onContentDurationTimeout();

    bool shouldBeContentReplaced();
    size_t getNextContentIndex();

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_handler;
    int m_id;
    int m_zorder;
    bool m_contentLooped = false;

    std::vector<std::unique_ptr<IRegionContent>> m_content;
    size_t m_currentContentIndex = 0;

};
