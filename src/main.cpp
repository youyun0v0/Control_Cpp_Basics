#include <iostream>

#include "control_basics/MathUtils.h"

int main() {
    const double samples1[] = {1.0, 5.0 ,3.0};
    const double samples2[] = {-2.0, -5.0 ,-1.0};
    const double max1 = control_basics::max_value(samples1,3);
    const double max2 = control_basics::max_value(samples2,3);
    std::cout<<"max1="<<max1<<std::endl;
    std::cout<<"max2="<<max2<<std::endl;
    return 0;
}