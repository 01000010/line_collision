#ifndef COLLISION_H
#define COLLISION_H

#include <optional>
#include <vector>
#include "line.h"

namespace lc
{
    std::optional<Point> get_collision_point(const Line& l1, const Line& l2);
}

#endif