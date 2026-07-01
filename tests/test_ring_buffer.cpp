#include <cassert>
#include "control_basics/RingBuffer.h"
#include <cmath>

namespace {

bool nearly_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 1e-9;
}
}


int main() {
    control_basics::RingBuffer ringbuffer;
    assert(ringbuffer.empty() == true);
    assert(! ringbuffer.full());
    assert(ringbuffer.count() == 0);
    assert(ringbuffer.capacity() == 8);
    assert(nearly_equal(ringbuffer.latest(), 0.0));

    ringbuffer.push(1.0);
    assert(ringbuffer.count() == 1);
    assert(nearly_equal(ringbuffer.latest(), 1.0));

    for (size_t i = 2; i <= ringbuffer.capacity(); i++)
    {
        ringbuffer.push(static_cast<double>(i));
        assert(nearly_equal(ringbuffer.latest(), static_cast<double>(i)));
    }
    assert(ringbuffer.full() == true);
    assert(nearly_equal(ringbuffer.at(0), 1.0));
    assert(nearly_equal(ringbuffer.at(1), 2.0));
    assert(nearly_equal(ringbuffer.at(2), 3.0));
    assert(nearly_equal(ringbuffer.at(3), 4.0));
    assert(nearly_equal(ringbuffer.at(4), 5.0));
    assert(nearly_equal(ringbuffer.at(5), 6.0));
    assert(nearly_equal(ringbuffer.at(6), 7.0));
    assert(nearly_equal(ringbuffer.at(7), 8.0));
    assert(nearly_equal(ringbuffer.at(8), 0.0));
    
    ringbuffer.push(114514);
    assert(ringbuffer.count() == 8);
    assert(nearly_equal(ringbuffer.latest(), 114514.0));
    assert(nearly_equal(ringbuffer.at(0), 2.0));
    assert(nearly_equal(ringbuffer.at(1), 3.0));
    assert(nearly_equal(ringbuffer.at(2), 4.0));
    assert(nearly_equal(ringbuffer.at(3), 5.0));
    assert(nearly_equal(ringbuffer.at(4), 6.0));
    assert(nearly_equal(ringbuffer.at(5), 7.0));
    assert(nearly_equal(ringbuffer.at(6), 8.0));
    assert(nearly_equal(ringbuffer.at(7), 114514.0));
    assert(nearly_equal(ringbuffer.at(8), 0.0));

    ringbuffer.clear();
    assert(ringbuffer.empty() == true);
    assert(ringbuffer.count() == 0);
    assert(nearly_equal(ringbuffer.latest(), 0.0));
    
    return 0;
}