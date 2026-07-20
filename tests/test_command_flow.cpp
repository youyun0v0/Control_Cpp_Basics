#include "control_basics/CommandParser.h"
#include "control_basics/CommandExecutor.h"
#include "control_basics/CommandQueue.h"
#include <cassert>

void test_one_input_flow(){
    control_basics::CommandParser parser;
    control_basics::CommandQueue queue;
    control_basics::CommandExecutor executor;
    control_basics::MotionStateMachine machine;
    control_basics::ParseResult parseresult = parser.parse("START");
    assert(parseresult.status == control_basics::ParseStatus::Ok);
    assert(parseresult.command == control_basics::CommandType::Start);
    if (parseresult.status == control_basics::ParseStatus::Ok) {
        queue.push(parseresult.command);
    }
    control_basics::CommandType command;
    assert(queue.pop(command));
    assert(command == control_basics::CommandType::Start);
    control_basics::ExecuteResult execute_result = executor.execute(command, machine);
    assert(execute_result.state == control_basics::MotionState::Running);
    assert(execute_result.status == control_basics::ExecuteStatus::Ok);
    assert(execute_result.error == control_basics::ErrorCode::None);
}

void test_invalid_input_flow(){
    control_basics::CommandParser parser;
    control_basics::CommandQueue queue;
    control_basics::CommandExecutor executor;
    control_basics::MotionStateMachine machine;
    control_basics::ParseResult parseresult = parser.parse("youyun");
    assert(parseresult.status == control_basics::ParseStatus::Unknown);
    assert(parseresult.command == control_basics::CommandType::Unknown);
    if (parseresult.status == control_basics::ParseStatus::Ok) {
        queue.push(parseresult.command);
    }
    assert(queue.empty());
}

void test_multiple_input_flow(){
    control_basics::CommandParser parser;
    control_basics::CommandQueue queue;
    control_basics::CommandExecutor executor;
    control_basics::MotionStateMachine machine;
    control_basics::ParseResult parseresult = parser.parse(" start\r\n");
    assert(parseresult.status == control_basics::ParseStatus::Ok);
    assert(parseresult.command == control_basics::CommandType::Start);
    if (parseresult.status == control_basics::ParseStatus::Ok) {
        queue.push(parseresult.command);
    }

    parseresult = parser.parse(" StATus");
    assert(parseresult.status == control_basics::ParseStatus::Ok);
    assert(parseresult.command == control_basics::CommandType::Status);
    if (parseresult.status == control_basics::ParseStatus::Ok) {
        queue.push(parseresult.command);
    }

    parseresult = parser.parse(" STOP  ");
    assert(parseresult.status == control_basics::ParseStatus::Ok);
    assert(parseresult.command == control_basics::CommandType::Stop);
    if (parseresult.status == control_basics::ParseStatus::Ok) {
        queue.push(parseresult.command);
    }

    control_basics::CommandType command;
    assert(queue.pop(command));
    assert(command == control_basics::CommandType::Start);
    control_basics::ExecuteResult execute_result = executor.execute(command, machine);
    assert(execute_result.state == control_basics::MotionState::Running);
    assert(execute_result.status == control_basics::ExecuteStatus::Ok);
    assert(execute_result.error == control_basics::ErrorCode::None);

    assert(queue.pop(command));
    assert(command == control_basics::CommandType::Status);
    execute_result = executor.execute(command, machine);
    assert(execute_result.state == control_basics::MotionState::Running);
    assert(execute_result.status == control_basics::ExecuteStatus::Ok);
    assert(execute_result.error == control_basics::ErrorCode::None);

    assert(queue.pop(command));
    assert(command == control_basics::CommandType::Stop);
    execute_result = executor.execute(command, machine);
    assert(execute_result.state == control_basics::MotionState::Stopped);
    assert(execute_result.status == control_basics::ExecuteStatus::Ok);
    assert(execute_result.error == control_basics::ErrorCode::None);
}

int main(){
    test_one_input_flow();
    test_invalid_input_flow();
    test_multiple_input_flow();
    return 0;
}