#include "control_basics/SignalUtils.h"
#include <cmath>

namespace control_basics
{
    double deadband(double value, double threshold)
    {
        if(std::fabs(value) <= std::fabs(threshold)) return 0.0;
        return value;
    }
}