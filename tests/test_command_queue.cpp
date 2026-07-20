#include "control_basics/CommandQueue.h"
#include <cassert>

void new_queue(){
    control_basics::CommandQueue q;
    assert(q.empty());
    assert(q.count() == 0);
}

void test_push_and_pop(){
    control_basics::CommandQueue q;
    assert(q.push(control_basics::CommandType::Start));
    assert(q.count() == 1);
    assert(q.push(control_basics::CommandType::Stop));
    control_basics::CommandType type1,type2;
    assert(q.pop(type1));
    assert(type1 == control_basics::CommandType::Start);
    assert(q.pop(type2));
    assert(type2 == control_basics::CommandType::Stop);
}

void test_empty_pop(){
    control_basics::CommandQueue q;
    control_basics::CommandType t;
    assert(!q.pop(t));
    for(size_t i = 0; i < q.capacity(); i++){
        assert(q.push(control_basics::CommandType::Start));
    }
    assert(q.full());
    assert(! q.push(control_basics::CommandType::Start));
}

void test_clear(){
    control_basics::CommandQueue q;
    for(size_t i = 0; i < q.capacity(); i++){
        assert(q.push(control_basics::CommandType::Start));
    }
    q.clear();
    assert(q.empty());
    assert(q.count() == 0);
}

int main(){
    new_queue();
    test_push_and_pop();
    test_empty_pop();
    test_clear();
    return 0;
}