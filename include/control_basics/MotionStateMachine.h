#pragma once

namespace control_basics {

enum class MotionState {
    Idle,
    Running,
    Stopped,
    Error
};

enum class ErrorCode {
    None,
    LimitTriggered,
    SensorInvalid,
    EmergencyStop
};

class MotionStateMachine {
    public:
    MotionStateMachine();
    MotionState state() const;

    bool start();
    bool stop();

    void set_error(ErrorCode error);
    bool reset_error();
    ErrorCode error() const;
    bool is_error() const;

    void emergency_stop();
    bool can_start() const;

    private:
    MotionState state_;
    ErrorCode error_;
};

}  // namespace control_basics