#pragma once

#include "schedule/LayoutQueue.hpp"

using OverlaysIds = std::vector<int>;

class OverlayLayoutQueue : public LayoutQueue
{
public:
    OverlaysIds overlays() const  // TODO tests
    {
        OverlaysIds ids;

        for (auto&& layout : queue())
        {
            ids.push_back(layout.id);
        }

        return ids;
    }
};
