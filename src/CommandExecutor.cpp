#include "control_basics/CommandExecutor.h"

namespace{
    control_basics::ExecuteResult make_result(control_basics::ExecuteStatus status, const control_basics::MotionStateMachine& machine) {
        return {status, machine.state(), machine.error()};
    }
}

namespace control_basics
{
    ExecuteResult CommandExecutor::execute(CommandType command, MotionStateMachine& machine) const{
        if(command == CommandType::Start){
            if( machine.start() ){
                return make_result(ExecuteStatus::Ok, machine);
            }
            else return make_result(ExecuteStatus::Rejected, machine);
        }
        else if(command == CommandType::Stop){
            if( machine.stop()){
                return make_result(ExecuteStatus::Ok, machine);
            }
            else return make_result(ExecuteStatus::Rejected, machine);
        }
        else if(command == CommandType::Reset){
            if(machine.reset_error()){
                return make_result(ExecuteStatus::Ok, machine);
            }
            else return make_result(ExecuteStatus::Rejected, machine);
        }
        else if(command == CommandType::Status){
            return make_result(ExecuteStatus::Ok,machine);
        }
        else{
            return make_result(ExecuteStatus::InvalidCommand, machine);
        } 
    }
}