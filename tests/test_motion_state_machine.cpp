#include <cassert>
#include "control_basics/MotionStateMachine.h"

void test_initial_state(){
    control_basics::MotionStateMachine machine;
    assert(machine.state() == control_basics::MotionState::Idle);
    assert(machine.can_start() == true);
}
void test_start_and_stop(){
    control_basics::MotionStateMachine machine;
    assert(machine.start());
    assert(machine.can_start() == false);
    assert(machine.stop());
    assert(machine.state() == control_basics::MotionState::Stopped);
    assert(machine.can_start() == true);
}
void test_invalid_stop_from_idle(){
    control_basics::MotionStateMachine machine;
    assert(!machine.stop());
    assert(machine.state() == control_basics::MotionState::Idle);
}
void test_error_blocks_start(){
    control_basics::MotionStateMachine machine;
    machine.set_error(control_basics::ErrorCode::LimitTriggered);
    assert(!machine.start());
    assert(machine.state() == control_basics::MotionState::Error);
}
void test_emergency_stop_and_recover(){
    control_basics::MotionStateMachine machine;
    assert(machine.start());
    machine.emergency_stop();
    assert(!machine.start());
    assert(machine.state() == control_basics::MotionState::Error);
    assert(machine.error() == control_basics::ErrorCode::EmergencyStop);
    assert(machine.reset_error());
    assert(machine.state() == control_basics::MotionState::Idle);
}

int main(){
    test_initial_state();
    test_start_and_stop();
    test_invalid_stop_from_idle();
    test_error_blocks_start();
    test_emergency_stop_and_recover();
    return 0;

}