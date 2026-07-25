#include <cassert>
#include <cmath>

#include "control_basics/MotionController.h"

namespace {

bool nearly_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 1e-9;
}

}  // namespace

void test_default_controller_state() {
    control_basics::MotionController controller;
    assert(nearly_equal(controller.target_speed(), 100.0));
    assert(nearly_equal(controller.current_x(), 0.0));
    assert(nearly_equal(controller.current_y(), 0.0));
    assert(controller.state() == control_basics::MotionState::Idle);
}

void test_set_speed_updates_target_speed(){
    control_basics::MotionController controller;
    control_basics::CommandParser parser;
    auto parsed = parser.parse("SET_SPEED 120");
    control_basics::TrajectoryPlanner::Trajectory2D output;
    auto result = controller.handle(parsed, output);
    assert(nearly_equal(controller.target_speed(), 120.0));
    assert(result.trajectory_count == 0);
}

void test_set_speed_rejects_missing_argument(){
    control_basics::MotionController controller;
    control_basics::CommandParser parser;
    auto parsed = parser.parse("SET_SPEED ");
    control_basics::TrajectoryPlanner::Trajectory2D output;
    assert(controller.handle(parsed, output).status == control_basics::ControllerStatus::InvalidCommand);
}

void test_move_generates_trajectory(){
    control_basics::MotionController controller;
    control_basics::CommandParser parser;
    auto parsed = parser.parse("MOVE 30 40");
    control_basics::TrajectoryPlanner::Trajectory2D output1, output2;
    auto result = controller.handle(parsed, output1);
    assert(result.trajectory_count > 0);
    assert(nearly_equal(output1[result.trajectory_count - 1].x, 30.0));
    assert(nearly_equal(output1[result.trajectory_count - 1].y, 40.0));
    assert(result.status == control_basics::ControllerStatus::Ok);
    assert(nearly_equal(controller.current_x(), 30.0));
    assert(nearly_equal(controller.current_y(), 40.0));
    
    auto parsed2 = parser.parse("MOVE 50 40");
    auto result2 = controller.handle(parsed2, output2);
    assert(nearly_equal(output2[0].x, 30.0));
    assert(nearly_equal(output2[0].y, 40.0));
    assert(result2.status == control_basics::ControllerStatus::Ok);
    assert(nearly_equal(controller.current_x(), 50.0));
    assert(nearly_equal(controller.current_y(), 40.0));
}

void test_home(){
    control_basics::MotionController controller;
    control_basics::CommandParser parser;
    auto parsed = parser.parse("MOVE 30 40");
    control_basics::TrajectoryPlanner::Trajectory2D output1, output2;
    auto result = controller.handle(parsed, output1);
    assert(result.trajectory_count > 0);
    assert(nearly_equal(output1[result.trajectory_count - 1].x, 30.0));
    assert(nearly_equal(output1[result.trajectory_count - 1].y, 40.0));
    assert(result.status == control_basics::ControllerStatus::Ok);
    assert(nearly_equal(controller.current_x(), 30.0));
    assert(nearly_equal(controller.current_y(), 40.0));
    
    auto parsed2 = parser.parse("HOME");
    auto result2 = controller.handle(parsed2, output2);
    assert(result2.trajectory_count > 0);
    assert(result2.status == control_basics::ControllerStatus::Ok);
    assert(nearly_equal(controller.current_x(), 0.0));
    assert(nearly_equal(controller.current_y(), 0.0));
}

void test_status(){
    control_basics::MotionController controller;
    control_basics::CommandParser parser;
    control_basics::MotionState current_state = controller.state();
    auto parsed = parser.parse("STATUS");
    control_basics::TrajectoryPlanner::Trajectory2D output;
    auto result = controller.handle(parsed, output);
    assert(result.trajectory_count == 0);
    assert(controller.state() == current_state);
    assert(result.status == control_basics::ControllerStatus::Ok);
}

void test_start_and_stop(){
    control_basics::MotionController controller;
    control_basics::CommandParser parser;
    control_basics::MotionState current_state = controller.state();
    auto parsed = parser.parse("START");
    control_basics::TrajectoryPlanner::Trajectory2D output;
    auto result = controller.handle(parsed, output);
    assert(controller.state() == control_basics::MotionState::Running);
    assert(result.status == control_basics::ControllerStatus::Ok);

    auto parsed1 = parser.parse("STOP");
    control_basics::TrajectoryPlanner::Trajectory2D output1;
    auto result1 = controller.handle(parsed1, output1);
    assert(controller.state() == control_basics::MotionState::Stopped);
    assert(result1.status == control_basics::ControllerStatus::Ok);
}

void test_reset_does_not_generate_trajectory(){
    control_basics::MotionController controller;
    control_basics::CommandParser parser;
    control_basics::MotionState current_state = controller.state();
    auto parsed = parser.parse("RESET");
    control_basics::TrajectoryPlanner::Trajectory2D output;
    auto result = controller.handle(parsed, output);
    assert(result.trajectory_count == 0);
    assert(controller.state() == control_basics::MotionState::Idle);
    assert(result.status == control_basics::ControllerStatus::Ok);
}

int main() {
    test_default_controller_state();
    test_set_speed_updates_target_speed();
    test_set_speed_rejects_missing_argument();
    test_move_generates_trajectory();
    test_home();
    test_status();
    test_start_and_stop();
    test_reset_does_not_generate_trajectory();
    return 0;
}