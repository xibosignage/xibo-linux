#ifndef MEDIA_HPP
#define MEDIA_HPP

#include <string>

class Media
{
public:

    enum class Render
    {
        HTML,
        Native,
        Invalid
    };

    Media(int id, int duration, Render render, const std::string& uri);
    virtual ~Media() = default;

    int id() const;
    int duration() const;
    Render render() const;
    const std::string& uri() const;

protected:
    int m_id;
    int m_duration;
    Render m_render;
    std::string m_uri;

};

#endif // MEDIA_HPP
