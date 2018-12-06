#pragma once

#include "IMedia.hpp"
#include "utils/Observable.hpp"

#include <memory>

class Media : public Observable<IMedia>
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
