#pragma once

namespace control_basics {

enum class MotionState {
    Idle,
    Running,
    Stopped,
    Error
};//机器状态

enum class ErrorCode {
    None,
    LimitTriggered,
    SensorInvalid,
    EmergencyStop
};//错误码

class MotionStateMachine {
    public:
    MotionStateMachine(); //初始化

    //查询类：状态，错误码，是否错误，是否可以启动
    MotionState state() const;
    ErrorCode error() const;
    bool is_error() const;
    bool can_start() const;

    //操作类：开启，停止，设置错误码，错误清除，急停
    bool start();
    bool stop();
    void set_error(ErrorCode error);
    bool reset_error();
    void emergency_stop();
    

    private:
    MotionState state_;
    ErrorCode error_;
};

}  // namespace control_basics