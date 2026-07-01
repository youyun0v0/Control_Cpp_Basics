#pragma once
#include <cstddef>
#include "control_basics/RingBuffer.h"

namespace control_basics {

class MovingAverageFilter {
public:
    MovingAverageFilter();

    double update(double input);
    double output() const;
    void reset();
    std::size_t count() const;

private:
    RingBuffer buffer_;
    double output_;
};

}  // namespace control_basics