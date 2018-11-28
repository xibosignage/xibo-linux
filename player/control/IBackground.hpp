#pragma once

class IImageAdaptor;

class IBackground
{
public:
    virtual ~IBackground() = default;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void scale(double scaleX, double scaleY) = 0;

    virtual void show() = 0;
    virtual IImageAdaptor& handler() = 0;
};
