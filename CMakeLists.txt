cmake_minimum_required(VERSION 3.20)

project(week01_basic_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(control_basics
    src/MathUtils.cpp
    src/SignalUtils.cpp
)
# 生成静态库,里面包含了我们写的数学工具函数
# 库是供主程序和测试程序调用的工具函数

target_include_directories(control_basics
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
# 让主程序和测试程序都能找到头文件：头文件在include目录下的control_basics文件夹里

add_executable(day01_app
    src/main.cpp
)
# 生成主程序，主程序在src目录下的main.cpp里

add_executable(day01_test
    tests/test_math_utils.cpp
)
# 生成测试程序,没输出就说明测试通过

add_executable(test_signalutils
    tests/test_signal_utils.cpp
)

target_link_libraries(day01_app
    PRIVATE
        control_basics
)
# 主程序需要调用里面的函数

target_link_libraries(day01_test
    PRIVATE
        control_basics
)
# 测试程序也需要调用里面的函数

target_link_libraries(test_signalutils
    PRIVATE
        control_basics
)