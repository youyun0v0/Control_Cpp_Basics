//队列：先进先出
#pragma once
#include "control_basics/CommandParser.h"
#include <array>
#include <cstddef>

namespace control_basics {

class CommandQueue {
public:
    CommandQueue();

    bool push(CommandType command);
    bool pop(CommandType& command);
    void clear();

    std::size_t count() const;
    std::size_t capacity() const;
    bool empty() const;
    bool full() const;

private:
    static constexpr std::size_t kCapacity = 8;

    std::array<CommandType, kCapacity> data_;
    std::size_t head_;
    std::size_t tail_;
    std::size_t count_;
};

}  // namespace control_basics