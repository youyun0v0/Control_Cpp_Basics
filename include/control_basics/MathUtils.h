#pragma once

#include <cstddef>

namespace control_basics {

double clamp(double value, double min_value, double max_value);

double mean(const double* data, std::size_t size);

double max_value(const double* data, std::size_t size);

int normalize_pwm(double command);

double range(const double *data ,std::size_t size);

}  // namespace control_basics