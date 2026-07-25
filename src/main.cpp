#include <iostream>
#include <string>

#include "control_basics/CommandParser.h"
#include "control_basics/MathUtils.h"
#include "control_basics/LowPassFilter.h"
#include "control_basics/MotionController.h"
#include "control_basics/PIDController.h"
#include "control_basics/MovingAverageFilter.h"
#include "control_basics/MotionStateMachine.h"
#include "control_basics/TrajectoryCsvLogger.h"

namespace {
    //to string函数：把几个enum class转成字符串

const char* to_string(control_basics::CommandType command) {
    switch (command) {
    case control_basics::CommandType::Start:
        return "Start";
    case control_basics::CommandType::Stop:
        return "Stop";
    case control_basics::CommandType::Reset:
        return "Reset";
    case control_basics::CommandType::Status:
        return "Status";
    case control_basics::CommandType::SetSpeed:
        return "SetSpeed";
    case control_basics::CommandType::Move:
        return "Move";
    case control_basics::CommandType::Home:
        return "Home";
    case control_basics::CommandType::Unknown:
        return "Unknown";
    }

    return "Unknown";
}

const char* to_string(control_basics::ParseStatus status) {
    switch (status) {
    case control_basics::ParseStatus::Ok:
        return "Ok";
    case control_basics::ParseStatus::Empty:
        return "Empty";
    case control_basics::ParseStatus::TooFewArguments:
        return "TooFewArguments";
    case control_basics::ParseStatus::TooManyArguments:
        return "TooManyArguments";
    case control_basics::ParseStatus::InvalidArgument:
        return "InvalidArgument";
    case control_basics::ParseStatus::OutOfRange:
        return "OutOfRange";
    case control_basics::ParseStatus::Unknown:
        return "Unknown";
    }

    return "Unknown";
}

const char* to_string(control_basics::ControllerStatus status) {
    switch (status) {
    case control_basics::ControllerStatus::Ok:
        return "Ok";
    case control_basics::ControllerStatus::Rejected:
        return "Rejected";
    case control_basics::ControllerStatus::InvalidCommand:
        return "InvalidCommand";
    }

    return "InvalidCommand";
}

const char* to_string(control_basics::MotionState state) {
    switch (state) {
    case control_basics::MotionState::Idle:
        return "Idle";
    case control_basics::MotionState::Running:
        return "Running";
    case control_basics::MotionState::Stopped:
        return "Stopped";
    case control_basics::MotionState::Error:
        return "Error";
    }

    return "Unknown";
}

void print_trajectory_summary( //展示轨迹的基本信息
    const control_basics::TrajectoryPlanner::Trajectory2D& trajectory,
    std::size_t count) {
    std::cout << "trajectory_count=" << count << '\n';

    if (count == 0) {
        return;
    }

    const auto& first = trajectory[0];
    const auto& last = trajectory[count - 1];

    std::cout << "first point: "
              << "time=" << first.time
              << ", x=" << first.x
              << ", y=" << first.y
              << ", vx=" << first.vx
              << ", vy=" << first.vy << '\n';

    std::cout << "last point: "
              << "time=" << last.time
              << ", x=" << last.x
              << ", y=" << last.y
              << ", vx=" << last.vx
              << ", vy=" << last.vy << '\n';
}

void run_demo_command( //完整运行指令
    const std::string& input,
    control_basics::CommandParser& parser,
    control_basics::MotionController& controller,
    control_basics::TrajectoryCsvLogger& logger) {
    control_basics::TrajectoryPlanner::Trajectory2D trajectory{};

    std::cout << "\ncommand: " << input << '\n';
    //1.CommandParser解析指令
    const auto parsed = parser.parse(input);
    std::cout << "parse_status=" << to_string(parsed.status)
              << ", command_type=" << to_string(parsed.command)
              << ", argument_count=" << parsed.argument_count << '\n';

    if (parsed.argument_count > 0) {
        std::cout << "arguments:";
        for (std::size_t i = 0; i < parsed.argument_count; ++i) {
            std::cout << ' ' << parsed.arguments[i];
        }
        std::cout << '\n';
    }

    if (parsed.status != control_basics::ParseStatus::Ok) {
        std::cout << "controller skipped because parsing failed\n";
        return;
    }
    //2.MotionController执行指令
    const auto result = controller.handle(parsed, trajectory);

    std::cout << "controller_status=" << to_string(result.status)
              << ", target_speed=" << controller.target_speed()
              << ", current_position=(" << controller.current_x()
              << ", " << controller.current_y() << ")"
              << ", motion_state=" << to_string(controller.state()) << '\n';
    
    print_trajectory_summary(trajectory, result.trajectory_count);
    //3.有轨迹就生成csv
    if (result.trajectory_count > 0) {
        std::cout << "csv output:\n";
        logger.write_2d(std::cout, trajectory, result.trajectory_count);
    }
}

void run_final_system_demo() {
    control_basics::CommandParser parser;
    control_basics::MotionController controller;
    control_basics::TrajectoryCsvLogger logger;

    std::cout << "\n=== Final system demo ===\n";
    std::cout << "Pipeline: input string -> CommandParser -> MotionController -> TrajectoryPlanner -> TrajectoryCsvLogger\n";

    run_demo_command("SET_SPEED 80", parser, controller, logger);
    run_demo_command("MOVE 30 40", parser, controller, logger);
    run_demo_command("STATUS", parser, controller, logger);
    run_demo_command("HOME", parser, controller, logger);
    run_demo_command("MOVE 30", parser, controller, logger);
}

}  // namespace

int main() {
    //最大值函数演示
    const double samples1[] = {1.0, 5.0 ,3.0};
    const double samples2[] = {-2.0, -5.0 ,-1.0};
    const double max1 = control_basics::max_value(samples1,3);
    const double max2 = control_basics::max_value(samples2,3);
    std::cout<<"max1="<<max1<<std::endl;
    std::cout<<"max2="<<max2<<std::endl;
    //低通滤波器演示
    control_basics::LowPassFilter filter(0.5);
    filter.update(10.0);
    filter.update(14.0);
    filter.update(16.0);
    std::cout<<"Filter output="<<filter.output()<<std::endl;
    //PID计算演示
    control_basics::PIDController pid(5.0, 20.0, 10.0);
    pid.set_integral_limits(-100, 100);
    pid.set_output_limits(-100, 100);
    std::cout<<"pid output="<<pid.update(10.0, 20.0, 0.1)<<std::endl;
    //移动平均演示
    control_basics::MovingAverageFilter MAF;
    std::cout<<"input 10. Average="<<MAF.update(10.0)<<std::endl;
    std::cout<<"input 12. Average="<<MAF.update(12.0)<<std::endl;
    std::cout<<"input 11. Average="<<MAF.update(11.0)<<std::endl;
    std::cout<<"input 50. Average="<<MAF.update(50.0)<<std::endl;
    std::cout<<"input 12. Average="<<MAF.update(12.0)<<std::endl;
    std::cout<<"input 11. Average="<<MAF.update(11.0)<<std::endl;
    //状态机演示
    control_basics::MotionStateMachine motion;
    std::cout << "can start: " << (motion.can_start() ? "true":"false") << '\n';
    motion.start();
    std::cout << "can start after running: " << (motion.can_start() ? "true":"false") << '\n';
    motion.emergency_stop();
    std::cout << "is error: " << (motion.is_error() ? "true":"false") << '\n';

    run_final_system_demo();

    return 0;
}
