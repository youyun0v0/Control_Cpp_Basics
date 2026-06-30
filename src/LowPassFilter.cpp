#include "control_basics/LowPassFilter.h"
#include "control_basics/MathUtils.h"

namespace control_basics
{
    LowPassFilter::LowPassFilter(double alpha)
        :alpha_(clamp(alpha, 0.0, 1.0)),
        previous_output_(0.0),
        initialized_(false)
        {
        }
    double LowPassFilter::update(double input){
        if(! initialized_){
            initialized_ = true;
            previous_output_ = input;
            return input;
        }
        else{
            previous_output_= alpha_ *input + (1- alpha_ )* previous_output_;
            return previous_output_;
        }
    }
    double LowPassFilter::output() const {
        return previous_output_;
    }
    void LowPassFilter::reset(){
        previous_output_ = 0;
        initialized_ = false;
    }
}