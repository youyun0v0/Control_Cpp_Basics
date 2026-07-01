#include <cstddef>
#include <array>
#include <cassert>
#include <cmath>

template <std::size_t Size>
double mean_array(const std::array<double, Size>& data){
    if (Size == 0) return 0.0;
    double sum = 0.0;
    for (std::size_t i = 0; i < Size; i++){
        sum += data[i];
    }
    return sum / static_cast<double>(Size);
}

bool nearly_equal(double lhs, double rhs) {
    return std::fabs(lhs - rhs) < 1e-9;
}

int main(){
    const std::array<double, 4> array1 = {1.0, 2.0, 3.0, 4.0};
    const std::array<double, 2> array2 = {10.0, 20.0};
    assert(nearly_equal(mean_array(array1),2.5));
    assert(nearly_equal(mean_array(array2),15.0));

    return 0;
}