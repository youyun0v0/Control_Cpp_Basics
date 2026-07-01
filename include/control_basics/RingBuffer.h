#pragma once
#include <cstddef>
#include <array>

namespace control_basics {

class RingBuffer {
public:
    RingBuffer();

    void push(double value);
    void clear();

    std::size_t count() const;//查询函数，不会修改成员内部的变量
    std::size_t capacity() const;
    bool empty() const;
    bool full() const;

    double latest() const;
    double at(std::size_t index) const;

private:
    static constexpr std::size_t kCapacity = 8;

    std::array<double, kCapacity> data_;
    std::size_t write_index_; //无符号整数
    std::size_t count_;
};

}  // namespace control_basics