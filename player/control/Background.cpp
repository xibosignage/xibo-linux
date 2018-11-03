#include "Background.hpp"

#include <cassert>

Background::Background(int width, int height, std::unique_ptr<IImageAdaptor>&& handler) :
    m_handler(std::move(handler))
{
    assert(m_handler);

    m_handler->setSize(width, height);
}

int Background::width() const
{
    return m_handler->width();
}

int Background::height() const
{
    return m_handler->height();
}

void Background::scale(double scaleX, double scaleY)
{
    m_handler->scale(scaleX, scaleY);
}

void Background::show()
{
    m_handler->show();
}

IImageAdaptor& Background::handler()
{
    return *m_handler;
}
