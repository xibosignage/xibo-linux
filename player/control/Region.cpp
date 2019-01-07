#include "Region.hpp"

#include "control/IRegionContent.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"

#include <cassert>

const int FIRST_CONTENT_INDEX = 0;


Region::Region(int id, int width, int height, int zorder, std::unique_ptr<IFixedLayoutAdaptor>&& handler) :
    m_handler(std::move(handler)), m_id(id), m_zorder(zorder)
{
    assert(m_handler);

    m_handler->setSize(width, height);
}

Region::~Region()
{
}

int Region::width() const
{
    return m_handler->width();
}

int Region::height() const
{
    return m_handler->height();
}

void Region::scale(double scaleX, double scaleY)
{
    assert(m_content.size() > 0);

    m_handler->scale(scaleX, scaleY);
    scaleContent(scaleX, scaleY);
}

void Region::loopContent()
{
    m_contentLooped = true;
}

bool Region::contentLooped() const
{
    return m_contentLooped;
}

int Region::id() const
{
    return m_id;
}

void Region::scaleContent(double scaleX, double scaleY)
{
    for(auto&& content : m_content)
    {
        content->scale(scaleX, scaleY);
    }
}

int Region::zorder() const
{
    return m_zorder;
}

IFixedLayoutAdaptor& Region::handler()
{
    return *m_handler;
}

void Region::show()
{
    assert(m_content.size() > 0);

    m_handler->show();
    placeContent(FIRST_CONTENT_INDEX);
}

void Region::placeContent(size_t contentIndex)
{
    m_currentContentIndex = contentIndex;
    m_content[contentIndex]->start();
}

void Region::removeContent(size_t contentIndex)
{
    m_content[contentIndex]->stop();
}

void Region::addContent(std::unique_ptr<IRegionContent>&& content, int x, int y)
{
    m_handler->addChild(content->handler(), x, y);

    content->subscribe(EventType::DurationExpired, std::bind(&Region::onContentDurationTimeout, this));
    m_content.push_back(std::move(content));
}

void Region::onContentDurationTimeout()
{
    if(isExpired())
    {
        pushEvent(RegionDurationExpiredEvent{m_id});
    }

    if(shouldBeContentReplaced())
    {
        removeContent(m_currentContentIndex);
        placeContent(getNextContentIndex());
    }
}

bool Region::isExpired() const
{
    return getNextContentIndex() == FIRST_CONTENT_INDEX;
}

bool Region::shouldBeContentReplaced() const
{
    return m_content.size() > 1 || m_contentLooped;
}

size_t Region::getNextContentIndex() const
{
    size_t nextContentIndex = m_currentContentIndex + 1;

    if(nextContentIndex >= m_content.size())
        return FIRST_CONTENT_INDEX;

    return nextContentIndex;
}
