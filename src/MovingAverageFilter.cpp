#include "control_basics/MovingAverageFilter.h"
#include "control_basics/MathUtils.h"

namespace control_basics
{
    MovingAverageFilter::MovingAverageFilter()
        :output_(0.0){
        }
    double MovingAverageFilter::update(double input){
        buffer_.push(input);
        double sum = 0.0;
        std::size_t cnt =  buffer_.count();
        for(std::size_t i = 0; i < cnt; i++){
            sum += buffer_.at(i);
        }
        output_ = sum / static_cast<double>(cnt);
        return output_;
    }
    double MovingAverageFilter::output() const{
        return output_;
    }
    void MovingAverageFilter::reset(){
        buffer_.clear();
        output_ = 0;
    }
    std::size_t MovingAverageFilter::count() const{
        return buffer_.count();
    }
}