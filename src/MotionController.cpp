#include "control_basics/MotionController.h"

namespace control_basics
{
    ControllerResult MotionController::handle(
        const ParseResult& command,
        TrajectoryPlanner::Trajectory2D& output){
            ControllerResult result;
            if(command.command == CommandType::SetSpeed){
                if(command.argument_count != 1) return result;
                double speed = command.arguments[0];
                if(speed <= 0) return result;
                result.status = ControllerStatus::Ok;
                target_speed_ = speed;
                result.trajectory_count = 0; //未生成有效数据点
            }
            if(command.command == CommandType::Move){
                if(!machine_.can_start()){
                    result.status = ControllerStatus::Rejected;
                    return result;
                }
                if(command.argument_count != 2) return result;
                result = move_to(command.arguments[0], command.arguments[1], output);
            }
            if(command.command == CommandType::Home){
                if(!machine_.can_start()){
                    result.status = ControllerStatus::Rejected;
                    return result;
                }
                result = move_to(0.0, 0.0, output);
            }
            if(command.command == CommandType::Reset){
                result.status = machine_.reset_error() ? ControllerStatus::Ok : ControllerStatus::Rejected;
            }
            if(command.command == CommandType::Start){
                result.status = machine_.start() ? ControllerStatus::Ok : ControllerStatus::Rejected;
            }
            if(command.command == CommandType::Stop){
                result.status = machine_.stop() ? ControllerStatus::Ok : ControllerStatus::Rejected;
            }
            if(command.command == CommandType::Status){
                result.status = ControllerStatus::Ok;
            }
            return result;
        }

    double MotionController::target_speed() const{
        return target_speed_;
    }
    double MotionController::current_x() const{
        return current_x_;
    }
    double MotionController::current_y() const{
        return current_y_;
    }
    MotionState MotionController::state() const{
        return machine_.state();
    }
    ControllerResult MotionController::move_to(double target_x, double target_y, TrajectoryPlanner::Trajectory2D& output){
        ControllerResult result;
        std::size_t count = planner_.generate_line_2d(current_x_, current_y_, target_x, target_y, target_speed_, max_acceleration_, dt_, output);
        if (count == 0) {
            result.status = ControllerStatus::Rejected;
            result.trajectory_count = 0;
            return result;
        }
        machine_.start();
        result.trajectory_count = count;
        current_x_ = target_x;
        current_y_ = target_y;
        machine_.stop();
        result.status = ControllerStatus::Ok;
        return result;
    }
}