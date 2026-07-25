#pragma once

#include <cstddef>
#include <iosfwd>

#include "control_basics/TrajectoryPlanner.h"

namespace control_basics {

class TrajectoryCsvLogger {
public:
    void write_2d(
        std::ostream& output,
        const TrajectoryPlanner::Trajectory2D& trajectory,
        std::size_t count) const;
};

}  // namespace control_basics