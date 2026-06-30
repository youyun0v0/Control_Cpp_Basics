#include "control_basics/Counter.h"
namespace control_basics
{
    Counter::Counter() //构造函数，名字与类名相同，无返回值
        : count_(0) {
    }//冒号后面是初始化列表，用于初始化成员变量
    Counter::Counter(int initial_value)
        : count_(initial_value){
    }
    void Counter::increment(){
        count_++;
    }
    void Counter::reset(){
        count_ = 0;
    }
    int Counter::value() const {
        return count_;
    }

}