#include <cassert>
#include <cmath>
#include "control_basics/LowPassFilter.h"


namespace {

bool nearly_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 1e-9;
}
}

int main() {
    control_basics::LowPassFilter filter(0.5);
    assert(nearly_equal(filter.update(10.0),10.0));
    assert(nearly_equal(filter.update(14.0),12.0));
    assert(nearly_equal(filter.update(16.0),14.0));
    filter.reset();
    assert(nearly_equal(filter.update(20.0),20.0));
    
    control_basics::LowPassFilter filter_a(0.5);
    control_basics::LowPassFilter filter_b(0.5);
    assert(nearly_equal(filter_a.update(10.0),10.0));
    assert(nearly_equal(filter_b.update(100.0),100.0));
    assert(nearly_equal(filter_a.update(14.0),12.0));
    assert(nearly_equal(filter_b.update(120.0),110.0));

    filter_a.reset();
    assert(nearly_equal(filter_a.output(), 0));
    assert(nearly_equal(filter_a.update(50.0),50.0));
    return 0;
}