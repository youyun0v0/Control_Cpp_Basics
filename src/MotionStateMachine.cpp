#include "control_basics/MotionStateMachine.h"
namespace control_basics
{
    MotionStateMachine::MotionStateMachine()
        :state_(MotionState::Idle)
        ,error_(ErrorCode::None){
        }
    MotionState MotionStateMachine::state() const{
        return state_;
    }
    bool MotionStateMachine::start(){
        if(MotionStateMachine::can_start()){
            state_ = MotionState::Running;
            return true;
        }
        return false;
    }
    bool MotionStateMachine::stop(){
        if(state_ == MotionState::Running){
            state_ = MotionState::Stopped;
            return true;
        }
        return false;
    }

    void MotionStateMachine::set_error(ErrorCode error){
        error_ = error;
        state_ = MotionState::Error;
    }
    bool MotionStateMachine::reset_error(){
        if(state_ == MotionState::Error){
            state_ = MotionState::Idle;
            error_ = ErrorCode::None;
            return true;
        }
        return false;
    }
    ErrorCode MotionStateMachine::error() const{
        return error_;
    }
    bool MotionStateMachine::is_error() const{
        return state_ == MotionState::Error;
    }

    void MotionStateMachine::emergency_stop(){
        set_error(ErrorCode::EmergencyStop); 
    }

    bool MotionStateMachine::can_start() const{
        if(state_ == MotionState::Idle || state_ == MotionState::Stopped) return true;
        return false;
    }
}