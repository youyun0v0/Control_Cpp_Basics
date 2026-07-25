cmake_minimum_required(VERSION 3.20)

project(week01_basic_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(control_basics
    src/MathUtils.cpp
    src/SignalUtils.cpp
    src/Counter.cpp
    src/LowPassFilter.cpp
    src/PIDController.cpp
    src/RingBuffer.cpp
    src/MovingAverageFilter.cpp
    src/MotionStateMachine.cpp
    src/CommandParser.cpp
    src/CommandExecutor.cpp
    src/CommandQueue.cpp
    src/TrajectoryPlanner.cpp
    src/MotionController.cpp
    src/TrajectoryCsvLogger.cpp
)
# 生成静态库,里面包含了我们写的数学工具函数
# 库是供主程序和测试程序调用的工具函数

target_include_directories(control_basics
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
# 让主程序和测试程序都能找到头文件：头文件在include目录下的control_basics文件夹里

add_executable(main_test
    src/main.cpp
)
# 生成主程序，主程序在src目录下的main.cpp里

add_executable(test_math_utils
    tests/test_math_utils.cpp
)
# 生成测试程序,没输出就说明测试通过

add_executable(test_signalutils
    tests/test_signal_utils.cpp
)

add_executable(test_counter
    tests/test_counter.cpp
)

add_executable(test_LowPassFilter
    tests/test_lowpassfilter.cpp
)

add_executable(test_pid_controller
    tests/test_pid_controller.cpp
)

add_executable(test_array_basics
    tests/test_array_basics.cpp
)

add_executable(test_ring_buffer
    tests/test_ring_buffer.cpp
)

add_executable(test_moving_average_filter
    tests/test_moving_average_filter.cpp
)

add_executable(test_motion_state_machine
    tests/test_motion_state_machine.cpp
)

add_executable(test_command_parser
    tests/test_command_parser.cpp
)

add_executable(test_command_executor
    tests/test_command_executor.cpp
)

add_executable(test_command_queue
    tests/test_command_queue.cpp
)

add_executable(test_command_flow
    tests/test_command_flow.cpp
)

add_executable(test_trajectory_planner
    tests/test_trajectory_planner.cpp
)

add_executable(test_motion_controller
    tests/test_motion_controller.cpp
)

add_executable(test_trajectory_csv_logger
    tests/test_trajectory_csv_logger.cpp
)

add_executable(test_system_flow
    tests/test_system_flow.cpp
)

target_link_libraries(main_test
    PRIVATE
        control_basics
)
# 主程序需要调用里面的函数

target_link_libraries(test_math_utils
    PRIVATE
        control_basics
)
# 测试程序也需要调用里面的函数

target_link_libraries(test_signalutils
    PRIVATE
        control_basics
)

target_link_libraries(test_counter
    PRIVATE
        control_basics
)

target_link_libraries(test_LowPassFilter
    PRIVATE
        control_basics
)

target_link_libraries(test_pid_controller
    PRIVATE
        control_basics
)

target_link_libraries(test_ring_buffer
    PRIVATE
        control_basics
)

target_link_libraries(test_moving_average_filter
    PRIVATE
        control_basics
)

target_link_libraries(test_motion_state_machine
    PRIVATE
        control_basics
)

target_link_libraries(test_command_parser
    PRIVATE
        control_basics
)

target_link_libraries(test_command_executor
    PRIVATE
        control_basics
)

target_link_libraries(test_command_queue
    PRIVATE
        control_basics
)

target_link_libraries(test_command_flow
    PRIVATE
        control_basics
)

target_link_libraries(test_trajectory_planner
    PRIVATE
        control_basics
)

target_link_libraries(test_motion_controller
    PRIVATE
        control_basics
)

target_link_libraries(test_trajectory_csv_logger
    PRIVATE
        control_basics
)

target_link_libraries(test_system_flow
    PRIVATE
        control_basics
)