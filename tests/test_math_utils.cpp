#include <cassert>
#include <cmath>

#include "control_basics/MathUtils.h"

namespace {

bool nearly_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 1e-9;
}

}  // namespace

int main() {
    using control_basics::clamp;
    using control_basics::mean;
    using control_basics::max_value;
    using control_basics::normalize_pwm;
    using control_basics::range;

    assert(nearly_equal(clamp(5.0, -10.0, 10.0), 5.0));
    assert(nearly_equal(clamp(15.0, -10.0, 10.0), 10.0));
    assert(nearly_equal(clamp(-15.0, -10.0, 10.0), -10.0));

    const double samples[] = {1.0, 2.0, 3.0, 4.0};
    assert(nearly_equal(mean(samples, 4), 2.5));
    assert(nearly_equal(mean(nullptr, 0), 0.0));

    const double samples1[] = {1.0, 5.0, 3.0};
    const double samples2[] = {-2.0, -5.0, -1.0};
    assert(nearly_equal(max_value(samples1, 3), 5.0));
    assert(nearly_equal(max_value(samples2, 3), -1.0));
    assert(nearly_equal(max_value(nullptr, 0), 0.0));

    assert(normalize_pwm(0.5) == 500);
    assert(normalize_pwm(2.0) == 1000);
    assert(normalize_pwm(-1.5) == -1000);
    assert(normalize_pwm(0.0) == 0);

    const double samples3[] = {1.0, 3.0, 7.0, 2.0};
    const double samples4[] = {-5.0, -2.0, -9.0};
    const double samples5[] = {3.0};
    assert(nearly_equal(range(samples3, 4), 6.0));
    assert(nearly_equal(range(samples4, 3), 7.0));
    assert(nearly_equal(range(samples5, 1), 0.0));
    assert(nearly_equal(range(nullptr,0), 0.0));

    return 0;
}