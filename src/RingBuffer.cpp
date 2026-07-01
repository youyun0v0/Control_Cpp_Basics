#include "control_basics/RingBuffer.h"

namespace control_basics
{
    RingBuffer::RingBuffer()
        :data_{},
        write_index_(0),
        count_(0){
        }
    void RingBuffer::push(double value){
        data_[write_index_] = value;
        write_index_ = (write_index_ + 1) % kCapacity;
        if(count_ < kCapacity) count_ += 1;
    }
    void RingBuffer::clear(){
        write_index_ = 0;
        count_ = 0;
        for(std::size_t i = 0; i < kCapacity; i++){
            data_[i] = 0;
        }
    }

    std::size_t RingBuffer::count() const{
        return count_;
    }
    std::size_t RingBuffer::capacity() const{
        return kCapacity;
    }
    bool RingBuffer::empty() const{
        return count_ == 0;
    }
    bool RingBuffer::full() const{
        return count_ == kCapacity;
    }

    
    double RingBuffer::latest() const{
        if(empty()) return 0.0;
        std::size_t latest_index = (write_index_ - 1 + kCapacity) % kCapacity; //防止负数
        return data_[latest_index];
    }
    double RingBuffer::at(std::size_t index) const{
        if(index >= count_) return 0.0;
        if(full()) return data_[(write_index_ + index) % kCapacity];
        else return data_[index];
    }

}