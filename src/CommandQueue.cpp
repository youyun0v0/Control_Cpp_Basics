#include "control_basics/CommandQueue.h"

namespace{
    
}

namespace control_basics
{
    CommandQueue::CommandQueue()
    :data_(),
    head_(0),
    tail_(0),
    count_(0){
    }

    bool CommandQueue::push(CommandType command){
        if(count_ >= kCapacity || command == CommandType::Unknown) return false;
        data_[tail_] = command;
        tail_ += 1;
        tail_ %= kCapacity;
        count_ += 1;
        return true;
    }
    bool CommandQueue::pop(CommandType& command){
        if(count_ == 0) return false;
        command = data_[head_];
        head_ += 1;
        head_ %= kCapacity;
        count_ -= 1;
        return true;
    }
    void CommandQueue::clear(){
        head_ = 0;
        tail_ = 0;
        count_ = 0;
    }

    std::size_t CommandQueue::count() const{
        return count_;
    }

    std::size_t CommandQueue::capacity() const{
        return kCapacity;
    }

    bool CommandQueue::empty() const{
        return count_ == 0;
    }
    bool CommandQueue::full() const{
        return count_ == kCapacity;
    }
}