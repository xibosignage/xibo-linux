#include "Background.hpp"
#include "adaptors/IImageAdaptor.hpp"

#include <cassert>

Background::Background(std::unique_ptr<IImageAdaptor>&& handler) :
    m_handler(std::move(handler))
{
    assert(m_handler);
}

int Background::width() const
{
    return m_handler->width();
}

int Background::height() const
{
    return m_handler->height();
}

// TODO specify about background behaviour (stretch or scaled)
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
