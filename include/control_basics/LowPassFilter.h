#pragma once

namespace control_basics {

class LowPassFilter {
public:
    explicit LowPassFilter(double alpha);

    double update(double input);
    double output() const;
    void reset();

private:
    double alpha_;
    double previous_output_;
    bool initialized_;
};

}  // namespace control_basics