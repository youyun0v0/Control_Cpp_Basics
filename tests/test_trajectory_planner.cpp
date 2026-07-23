#include <cassert>
#include <cmath>

#include "control_basics/TrajectoryPlanner.h"
#include "control_basics/CommandParser.h"

namespace{
bool nearly_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 1e-9;
}
}


void test_invalid_parameters(){
    control_basics::TrajectoryPlanner::Trajectory output;
    control_basics::TrajectoryPlanner planner;
    assert(planner.generate_1d(0.0, 100.0, -1.0, 1.0, 1.0, output) == 0);
    assert(planner.generate_1d(0.0, 100.0, 1.0, -1.0, 1.0, output) == 0);
    assert(planner.generate_1d(0.0, 100.0, 1.0, 1.0, -1.0, output) == 0);
}
void test_zero_distance(){
    control_basics::TrajectoryPlanner::Trajectory output;
    control_basics::TrajectoryPlanner planner;
    assert(planner.generate_1d(50.0, 50.0, 1.0, 1.0, 1.0, output) == 1);
    assert(output[0].time == 0.0);
    assert(output[0].position == 50.0);
    assert(output[0].velocity == 0.0);
}

void test_forward_motion_reaches_target() {
    control_basics::TrajectoryPlanner planner;
    control_basics::TrajectoryPlanner::Trajectory output{};

    const std::size_t count =
        planner.generate_1d(0.0, 10.0, 3.0, 2.0, 0.1, output);

    assert(count > 2);

    assert(nearly_equal(output[0].position, 0.0));
    assert(nearly_equal(output[count - 1].position, 10.0));
    assert(nearly_equal(output[count - 1].velocity, 0.0));
}

void test_reverse_motion_reaches_target() {
    control_basics::TrajectoryPlanner planner;
    control_basics::TrajectoryPlanner::Trajectory output{};

    const std::size_t count =
        planner.generate_1d(10.0, 0.0, 3.0, 2.0, 0.1, output);

    assert(count > 2);

    assert(nearly_equal(output[0].position, 10.0));
    assert(nearly_equal(output[count - 1].position, 0.0));
    assert(nearly_equal(output[count - 1].velocity, 0.0));

    assert(output[1].position < output[0].position);
}

void test_short_motion_does_not_overshoot() {
    control_basics::TrajectoryPlanner planner;
    control_basics::TrajectoryPlanner::Trajectory output{};

    const std::size_t count =
        planner.generate_1d(0.0, 0.2, 10.0, 2.0, 0.01, output);

    assert(count > 1);

    for (std::size_t i = 0; i < count; ++i) {
        assert(output[i].position >= -1e-6);
        assert(output[i].position <= 0.2 + 1e-6);
    }

    assert(nearly_equal(output[count - 1].position, 0.2));
    assert(nearly_equal(output[count - 1].velocity, 0.0));
}

void test_2d_line_reaches_target(){
    control_basics::TrajectoryPlanner planner;
    control_basics::TrajectoryPlanner::Trajectory2D output{};

    const std::size_t count =
        planner.generate_line_2d(0.0, 0.0, 10.0, 20.0, 0.1, 0.01, 0.01, output);

    assert(count > 1);
    
    assert(nearly_equal(output[count - 1].x, 10.0));
    assert(nearly_equal(output[count - 1].y, 20.0));
    assert(nearly_equal(output[count - 1].vx, 0.0));
    assert(nearly_equal(output[count - 1].vy, 0.0));
}

void test_2d_zero_distance(){
    control_basics::TrajectoryPlanner planner;
    control_basics::TrajectoryPlanner::Trajectory2D output{};

    const std::size_t count =
        planner.generate_line_2d(10.0, 10.0, 10.0, 10.0, 0.1, 0.01, 0.01, output);

    assert(count == 1);
    assert(nearly_equal(output[count - 1].time, 0.0));
    assert(nearly_equal(output[count - 1].x, 10.0));
    assert(nearly_equal(output[count - 1].y, 10.0));
    assert(nearly_equal(output[count - 1].vx, 0.0));
    assert(nearly_equal(output[count - 1].vy, 0.0));
}

void test_move_command_can_feed_trajectory_planner(){
    control_basics::CommandParser parser;
    const control_basics::ParseResult result = parser.parse("MOVE 30 40");

    assert(result.command == control_basics::CommandType::Move);
    assert(result.status == control_basics::ParseStatus::Ok);
    assert(result.argument_count == 2);

    double target_x = result.arguments[0];
    double target_y = result.arguments[1];

    control_basics::TrajectoryPlanner planner;
    control_basics::TrajectoryPlanner::Trajectory2D output{};
    std::size_t count = planner.generate_line_2d(0.0, 0.0, target_x, target_y, 1.0, 1.0, 0.1, output);

    assert(count > 1);
    assert(nearly_equal(output[count - 1].x, 30.0));
    assert(nearly_equal(output[count - 1].y, 40.0));
}

int main() {
    test_invalid_parameters();
    test_zero_distance();
    test_forward_motion_reaches_target();
    test_reverse_motion_reaches_target();
    test_short_motion_does_not_overshoot();
    test_2d_line_reaches_target();
    test_2d_zero_distance();
    test_move_command_can_feed_trajectory_planner();
    return 0;
}
