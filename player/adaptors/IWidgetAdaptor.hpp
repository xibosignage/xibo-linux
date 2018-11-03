#pragma once

class IWidgetAdaptor
{
public:
    virtual ~IWidgetAdaptor() = default;

    virtual void scale(double scaleX, double scaleY) = 0;
    virtual void setSize(int width, int height) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool isShown() const = 0;

};
