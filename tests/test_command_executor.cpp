#include "control_basics/CommandExecutor.h"
#include <cassert>

void test_start_command(){
    control_basics::MotionStateMachine machine;
    control_basics::CommandExecutor executor;
    const control_basics::ExecuteResult result = executor.execute(control_basics::CommandType::Start,machine);
    assert(result.status == control_basics::ExecuteStatus::Ok);
    assert(result.state == control_basics::MotionState::Running);
    assert(result.error == control_basics::ErrorCode::None);
    assert(machine.state() == control_basics::MotionState::Running);
}

void test_stop_command(){
    control_basics::MotionStateMachine machine;
    control_basics::CommandExecutor executor;
    machine.start();
    const control_basics::ExecuteResult result = executor.execute(control_basics::CommandType::Stop,machine);
    assert(result.status == control_basics::ExecuteStatus::Ok);
    assert(result.state == control_basics::MotionState::Stopped);
    assert(result.error == control_basics::ErrorCode::None);
    assert(machine.state() == control_basics::MotionState::Stopped);
}

void test_reset_command(){
    control_basics::MotionStateMachine machine;
    control_basics::CommandExecutor executor;
    machine.set_error(control_basics::ErrorCode::SensorInvalid);
    const control_basics::ExecuteResult result = executor.execute(control_basics::CommandType::Reset,machine);
    assert(result.status == control_basics::ExecuteStatus::Ok);
    assert(result.state == control_basics::MotionState::Idle);
    assert(result.error == control_basics::ErrorCode::None);
    assert(machine.state() == control_basics::MotionState::Idle);
}

void test_status_command(){
    control_basics::MotionStateMachine machine;
    control_basics::CommandExecutor executor;
    machine.set_error(control_basics::ErrorCode::SensorInvalid);
    const control_basics::ExecuteResult result = executor.execute(control_basics::CommandType::Status,machine);
    assert(result.status == control_basics::ExecuteStatus::Ok);
    assert(result.state == control_basics::MotionState::Error);
    assert(result.error == control_basics::ErrorCode::SensorInvalid);
    assert(machine.state() == control_basics::MotionState::Error);
}

void test_invalid_command(){
    control_basics::MotionStateMachine machine;
    control_basics::CommandExecutor executor;
    machine.set_error(control_basics::ErrorCode::SensorInvalid);
    const control_basics::ExecuteResult result = executor.execute(control_basics::CommandType::Unknown,machine);
    assert(result.status == control_basics::ExecuteStatus::InvalidCommand);
    assert(result.state == control_basics::MotionState::Error);
    assert(result.error == control_basics::ErrorCode::SensorInvalid);
    assert(machine.state() == control_basics::MotionState::Error);
}

void test_rejected_command(){
    control_basics::MotionStateMachine machine;
    control_basics::CommandExecutor executor;
    machine.set_error(control_basics::ErrorCode::SensorInvalid);
    const control_basics::ExecuteResult result = executor.execute(control_basics::CommandType::Start,machine);
    assert(result.status == control_basics::ExecuteStatus::Rejected);
    assert(result.state == control_basics::MotionState::Error);
    assert(result.error == control_basics::ErrorCode::SensorInvalid);
    assert(machine.state() == control_basics::MotionState::Error);
}


int main(){
    test_start_command();
    test_stop_command();
    test_reset_command();
    test_invalid_command();
    test_status_command();
    test_rejected_command();
    return 0;
}