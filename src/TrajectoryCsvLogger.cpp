#include "control_basics/TrajectoryCsvLogger.h"

#include <ostream>

namespace control_basics {

void TrajectoryCsvLogger::write_2d(
    std::ostream& output,
    const TrajectoryPlanner::Trajectory2D& trajectory,
    std::size_t count) const {
    output << "time,x,y,vx,vy\n";

    for (std::size_t i = 0; i < count; ++i) {
        output
            << trajectory[i].time << ","
            << trajectory[i].x << ","
            << trajectory[i].y << ","
            << trajectory[i].vx << ","
            << trajectory[i].vy << "\n";
    }
}

}  // namespace control_basics