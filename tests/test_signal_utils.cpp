#include <cassert>
#include <cmath>

#include "control_basics/SignalUtils.h"

namespace {

bool nearly_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 1e-9;
}

}  // namespace

int main() {
    using control_basics::deadband;
    assert(nearly_equal(deadband(0.01, 0.05), 0.0));
    assert(nearly_equal(deadband(0.2, 0.05), 0.2));
    assert(nearly_equal(deadband(-0.2, 0.05), -0.2));
    assert(nearly_equal(deadband(0.01, -0.05), 0.0));
    assert(nearly_equal(deadband(0.05, 0.05), 0.0));
    assert(nearly_equal(deadband(-0.05, 0.05), 0.0));
    return 0;
}