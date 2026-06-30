#include "control_basics/PIDController.h"
#include "control_basics/MathUtils.h"

namespace control_basics
{
    PIDController::PIDController(double kp, double ki, double kd)
        :kp_(kp),
        ki_(ki),
        kd_(kd),
        integral_(0.0),
        previous_error_(0.0),
        has_previous_error_(false),
        output_min_(-1.0),
        output_max_(1.0),
        integral_min_(-1.0),
        integral_max_(1.0) 
        {
        }
    double PIDController::update(double setpoint, double measurement, double dt){
        if(dt <= 0.0) return 0.0;
        double e = setpoint - measurement;//误差
        integral_ += e * dt;
        integral_ = clamp(integral_, integral_min_, integral_max_);
        if(!has_previous_error_){
            has_previous_error_ = true;
            previous_error_ = e;
            return clamp(kp_ * e + ki_ * integral_, output_min_, output_max_);
        }
        else{
            double pid = kp_ * e + ki_ * integral_ + kd_ * (e - previous_error_) / dt;
            previous_error_ = e;
            return clamp(pid, output_min_, output_max_);
        }
    }
    void PIDController::reset(){
        integral_ = 0.0;
        previous_error_ = 0.0;
        has_previous_error_ = false;
        
    }
    
    void PIDController::set_output_limits(double min_output, double max_output){
        output_max_ = max_output;
        output_min_ = min_output;
    }//输出限幅
    void PIDController::set_integral_limits(double min_integral, double max_integral){
        integral_max_ = max_integral;
        integral_min_ = min_integral;
        integral_ = clamp(integral_, min_integral, max_integral);
    }//积分限幅

    double PIDController::integral() const {
        return integral_;
    }
    double PIDController::previous_error() const {
        return previous_error_;
    }
}