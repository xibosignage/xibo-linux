#ifndef MEDIA_HPP
#define MEDIA_HPP

#include <gtkmm/widget.h>

struct Point
{
    int x;
    int y;
};

struct Size
{
    int width;
    int height;
};

class Region;

class Media
{
public:
   Media() = default;
   virtual ~Media() { }

   virtual void init(Region* region, const Point& pos, const Size& size, int zindez) = 0;
   virtual void hide() = 0;
   virtual void show() = 0;
   virtual int GetX() const;
   virtual int GetY() const;
   virtual int GetZ() const;
   virtual int GetWidth() const;
   virtual int GetHeight() const;
   virtual bool IsVisible() const;

protected:
   Point m_pos;
   Size m_size;
   int m_zindex;
   bool m_visible = false;

};

#endif // MEDIA_HPP
