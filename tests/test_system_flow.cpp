#include <cassert>
#include <sstream>
#include <string>
#include <cmath>

#include "control_basics/MotionController.h"
#include "control_basics/TrajectoryCsvLogger.h"

namespace control_basics {

namespace {

bool nearly_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 1e-9;
}

struct CommandRunResult {
    ParseResult parsed;
    ControllerResult controller_result;
    TrajectoryPlanner::Trajectory2D trajectory{};
};

CommandRunResult run_command(
    CommandParser& parser,
    MotionController& controller,
    const std::string& input) {
    CommandRunResult result;

    result.parsed = parser.parse(input);

    if (result.parsed.status != ParseStatus::Ok) {
        return result;
    }

    result.controller_result = controller.handle(
        result.parsed,
        result.trajectory);

    return result;
}

}  // namespace

void test_string_move_generates_csv() {
    CommandParser parser;
    MotionController controller;
    TrajectoryCsvLogger logger;

    auto result = run_command(parser, controller, "MOVE 30 40");

    assert(result.parsed.status == ParseStatus::Ok);
    assert(result.parsed.command == CommandType::Move);
    assert(result.controller_result.status == ControllerStatus::Ok);
    assert(result.controller_result.trajectory_count > 0);

    std::size_t last_index = result.controller_result.trajectory_count - 1;
    assert(nearly_equal(result.trajectory[last_index].x, 30.0));
    assert(nearly_equal(result.trajectory[last_index].y, 40.0));

    std::ostringstream output;
    logger.write_2d(
        output,
        result.trajectory,
        result.controller_result.trajectory_count);

    std::string csv = output.str();

    assert(csv.find("time,x,y,vx,vy") != std::string::npos);
    assert(!csv.empty());
}

void test_set_speed_then_move() {
    CommandParser parser;
    MotionController controller;

    auto speed_result = run_command(parser, controller, "SET_SPEED 80");

    assert(speed_result.parsed.status == ParseStatus::Ok);
    assert(speed_result.parsed.command == CommandType::SetSpeed);
    assert(speed_result.controller_result.status == ControllerStatus::Ok);
    assert(speed_result.controller_result.trajectory_count == 0);
    assert(nearly_equal(controller.target_speed(), 80.0));

    auto move_result = run_command(parser, controller, "MOVE 30 40");

    assert(move_result.parsed.status == ParseStatus::Ok);
    assert(move_result.parsed.command == CommandType::Move);
    assert(move_result.controller_result.status == ControllerStatus::Ok);
    assert(move_result.controller_result.trajectory_count > 0);
    assert(nearly_equal(controller.target_speed(), 80.0));
    assert(nearly_equal(controller.current_x(), 30.0));
    assert(nearly_equal(controller.current_y(), 40.0));
}

void test_invalid_input_does_not_reach_controller() {
    CommandParser parser;
    MotionController controller;

    double old_x = controller.current_x();
    double old_y = controller.current_y();
    double old_speed = controller.target_speed();
    MotionState old_state = controller.state();

    auto result = run_command(parser, controller, "MOVE 30");

    assert(result.parsed.status != ParseStatus::Ok);
    assert(result.controller_result.status == ControllerStatus::InvalidCommand);
    assert(result.controller_result.trajectory_count == 0);

    assert(nearly_equal(controller.current_x(), old_x));
    assert(nearly_equal(controller.current_y(), old_y));
    assert(nearly_equal(controller.target_speed(), old_speed));
    assert(controller.state() == old_state);
}

void test_move_then_home_returns_to_zero() {
    CommandParser parser;
    MotionController controller;

    auto move_result = run_command(parser, controller, "MOVE 30 40");

    assert(move_result.parsed.status == ParseStatus::Ok);
    assert(move_result.controller_result.status == ControllerStatus::Ok);
    assert(move_result.controller_result.trajectory_count > 0);
    assert(nearly_equal(controller.current_x(), 30.0));
    assert(nearly_equal(controller.current_y(), 40.0));

    auto home_result = run_command(parser, controller, "HOME");

    assert(home_result.parsed.status == ParseStatus::Ok);
    assert(home_result.parsed.command == CommandType::Home);
    assert(home_result.controller_result.status == ControllerStatus::Ok);
    assert(home_result.controller_result.trajectory_count > 0);

    std::size_t last_index = home_result.controller_result.trajectory_count - 1;
    assert(nearly_equal(home_result.trajectory[last_index].x, 0.0));
    assert(nearly_equal(home_result.trajectory[last_index].y, 0.0));

    assert(nearly_equal(controller.current_x(), 0.0));
    assert(nearly_equal(controller.current_y(), 0.0));
}

}  // namespace control_basics

int main() {
    control_basics::test_string_move_generates_csv();
    control_basics::test_set_speed_then_move();
    control_basics::test_invalid_input_does_not_reach_controller();
    control_basics::test_move_then_home_returns_to_zero();
    return 0;
}