#pragma once

#include "PriorityLayoutQueue.hpp"
#include "SequentialLayoutQueue.hpp"

using RegularLayoutQueue = SequentialLayoutQueue<PriorityLayoutQueue<LayoutQueue>>;
using OverlayLayoutQueue = PriorityLayoutQueue<LayoutQueue>;
