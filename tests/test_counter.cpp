#include <cassert>

#include "control_basics/Counter.h"

int main() {
    control_basics::Counter counter;
    assert(counter.value() == 0);
    counter.increment();
    assert(counter.value() == 1);
    counter.increment();
    assert(counter.value() == 2);
    counter.reset();
    assert(counter.value() == 0);

    control_basics::Counter initialized_counter(10);
    assert(initialized_counter.value() == 10);
    initialized_counter.increment();
    assert(initialized_counter.value() == 11);
    initialized_counter.increment();
    assert(initialized_counter.value() == 12);
    initialized_counter.reset();
    assert(initialized_counter.value() == 0);
    return 0;
}