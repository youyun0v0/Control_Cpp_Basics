#pragma once

namespace control_basics {

    class Counter {
    public:
        Counter();
        explicit Counter(int initial_value);
        void increment();
        void reset();
        int value() const;

    private:
        int count_;
};
}  // namespace control_basics