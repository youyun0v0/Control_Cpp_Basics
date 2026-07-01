#include <cassert>
#include "control_basics/MovingAverageFilter.h"
#include <cmath>

namespace {

bool nearly_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 1e-9;
}
}


int main() {
    control_basics::MovingAverageFilter MAF;
    assert(nearly_equal(MAF.output(), 0.0));
    assert(nearly_equal(MAF.update(10.0), 10.0));
    assert(nearly_equal(MAF.update(20.0), 15.0));
    assert(nearly_equal(MAF.update(30.0), 20.0));
    assert(nearly_equal(MAF.update(40.0), 25.0));
    assert(nearly_equal(MAF.update(50.0), 30.0));
    assert(nearly_equal(MAF.update(60.0), 35.0));
    assert(nearly_equal(MAF.update(70.0), 40.0));
    assert(nearly_equal(MAF.update(80.0), 45.0));
    assert(nearly_equal(MAF.update(90.0), 55.0));
    assert(nearly_equal(MAF.update(100.0), 65.0));
    MAF.reset();
    assert(MAF.count() == 0);
    assert(nearly_equal(MAF.output(), 0.0));
    return 0;
}