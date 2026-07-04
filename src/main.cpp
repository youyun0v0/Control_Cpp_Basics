#include <iostream>

#include "control_basics/MathUtils.h"
#include "control_basics/LowPassFilter.h"
#include "control_basics/PIDController.h"
#include "control_basics/MovingAverageFilter.h"
#include "control_basics/MotionStateMachine.h"

int main() {
    //最大值函数演示
    const double samples1[] = {1.0, 5.0 ,3.0};
    const double samples2[] = {-2.0, -5.0 ,-1.0};
    const double max1 = control_basics::max_value(samples1,3);
    const double max2 = control_basics::max_value(samples2,3);
    std::cout<<"max1="<<max1<<std::endl;
    std::cout<<"max2="<<max2<<std::endl;
    //低通滤波器演示
    control_basics::LowPassFilter filter(0.5);
    filter.update(10.0);
    filter.update(14.0);
    filter.update(16.0);
    std::cout<<"Filter output="<<filter.output()<<std::endl;
    //PID计算演示
    control_basics::PIDController pid(5.0, 20.0, 10.0);
    pid.set_integral_limits(-100, 100);
    pid.set_output_limits(-100, 100);
    std::cout<<"pid output="<<pid.update(10.0, 20.0, 0.1)<<std::endl;
    //移动平均演示
    control_basics::MovingAverageFilter MAF;
    std::cout<<"input 10. Average="<<MAF.update(10.0)<<std::endl;
    std::cout<<"input 12. Average="<<MAF.update(12.0)<<std::endl;
    std::cout<<"input 11. Average="<<MAF.update(11.0)<<std::endl;
    std::cout<<"input 50. Average="<<MAF.update(50.0)<<std::endl;
    std::cout<<"input 12. Average="<<MAF.update(12.0)<<std::endl;
    std::cout<<"input 11. Average="<<MAF.update(11.0)<<std::endl;
    //状态机演示
    control_basics::MotionStateMachine motion;
    std::cout << "can start: " << (motion.can_start() ? "true":"false") << '\n';
    motion.start();
    std::cout << "can start after running: " << (motion.can_start() ? "true":"false") << '\n';
    motion.emergency_stop();
    std::cout << "is error: " << (motion.is_error() ? "true":"false") << '\n';
    return 0;
}