#pragma once

#include <cstddef>
#include <string>

#include "control_basics/CommandParser.h"
#include "control_basics/MotionStateMachine.h"
#include "control_basics/TrajectoryPlanner.h"

namespace control_basics {

enum class ControllerStatus {
    Ok,
    Rejected,
    InvalidCommand
};

struct ControllerResult {
    ControllerStatus status = ControllerStatus::InvalidCommand;
    std::size_t trajectory_count = 0;
    std::string message;
};

class MotionController {
public:
    ControllerResult handle(
        const ParseResult& command,
        TrajectoryPlanner::Trajectory2D& output);

    double target_speed() const;
    double current_x() const;
    double current_y() const;
    MotionState state() const;

private:
    MotionStateMachine machine_;
    TrajectoryPlanner planner_;
    double target_speed_ = 100.0;
    double current_x_ = 0.0;
    double current_y_ = 0.0;
    double max_acceleration_ = 100.0;
    double dt_ = 0.1;
    ControllerResult move_to(double target_x, double target_y, TrajectoryPlanner::Trajectory2D& output);
};

}  // namespace control_basics