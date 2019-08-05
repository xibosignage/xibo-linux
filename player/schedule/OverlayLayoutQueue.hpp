#pragma once

#include "LayoutQueue.hpp"

class OverlayLayoutQueue : public LayoutQueue
{
public:
    std::vector<int> next() const
    {
        std::vector<int> ids;

        for(auto&& layout : queue())
        {
            ids.push_back(layout.id);
        }

        return ids;
    }
};
