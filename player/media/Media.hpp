#pragma once

#include "IMedia.hpp"
#include "utils/Event.hpp"

#include <memory>

class Media : public IMedia
{
public:
    Media(int id);

    int id() const final;
    int duration() const final;
    void setDuration(int duration) final;

private:
    int m_id;
    int m_duration;

};
