#pragma once
#include "control_basics/CommandParser.h"
#include "control_basics/MotionStateMachine.h"

namespace control_basics {

enum class ExecuteStatus {
    Ok,
    Rejected,
    InvalidCommand
};//执行状态

struct ExecuteResult {
    ExecuteStatus status;
    MotionState state;
    ErrorCode error;
};//执行结果

class CommandExecutor {
public:
    ExecuteResult execute(CommandType command, MotionStateMachine& machine) const;
    //执行：把经由分析器分析好的指令真正执行到状态机
};

}  // namespace control_basics