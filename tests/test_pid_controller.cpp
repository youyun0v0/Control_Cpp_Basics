#include <cassert>
#include <cmath>
#include "control_basics/PIDController.h"


namespace {

bool nearly_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 1e-9;
}
}

int main() {
    //单独测试p
    control_basics::PIDController p_controller(2.0, 0.0, 0.0);
    p_controller.set_output_limits(-100.0, 100.0);
    assert(nearly_equal(p_controller.update(10.0, 7.0, 0.1), 6.0));
    //连续测试pi
    control_basics::PIDController pi_controller(2.0, 1.0, 0.0);
    pi_controller.set_output_limits(-100.0, 100.0);
    pi_controller.set_integral_limits(-10.0, 10.0);
    assert(nearly_equal(pi_controller.update(10.0, 7.0, 0.1), 6.3));
    assert(nearly_equal(pi_controller.update(10.0, 7.0, 0.1), 6.6));
    assert(nearly_equal(pi_controller.integral(), 0.6));
    //测试输出限幅
    control_basics::PIDController limited_output(10.0, 0.0, 0.0);
    limited_output.set_output_limits(-5.0, 5.0);
    assert(nearly_equal(limited_output.update(10.0, 0.0, 0.1), 5.0));
    //测试积分限幅
    control_basics::PIDController limited_integral(0.0, 1.0, 0.0);
    limited_integral.set_output_limits(-100.0, 100.0);
    limited_integral.set_integral_limits(-0.5, 0.5);
    limited_integral.update(10.0, 0.0, 0.1);
    limited_integral.update(10.0, 0.0, 0.1);
    assert(nearly_equal(limited_integral.integral(), 0.5));
    //单独测试d
    control_basics::PIDController d_controller(0.0, 0.0, 1.0);
    d_controller.set_output_limits(-100.0, 100.0);
    assert(nearly_equal(d_controller.update(10.0, 8.0, 0.1), 0.0));
    assert(nearly_equal(d_controller.update(10.0, 9.0, 0.1), -10.0));
    //dt <= 0 边界条件测试
    assert(nearly_equal(d_controller.update(10.0, 9.0, 0.0), 0.0));
    //测试reset是否正常，测试
    d_controller.reset();
    assert(nearly_equal(d_controller.integral(), 0.0));
    assert(nearly_equal(d_controller.previous_error(), 0.0));
}