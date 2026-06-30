# Control_Cpp_Basics

这个项目用于通过小型控制相关函数和控制模块重建 C++ 工程基础。目标不是写复杂算法，而是练习：

- `.h/.cpp` 文件拆分
- CMake 工程组织
- 函数与类的封装
- 简单控制模块实现
- `assert` 测试
- 可复现的编译、运行和验证流程

## 文件结构

```text
control_cpp_basics/
  CMakeLists.txt
  include/
    control_basics/
      Counter.h
      LowPassFilter.h
      MathUtils.h
      PIDController.h
      SignalUtils.h
  src/
    Counter.cpp
    LowPassFilter.cpp
    MathUtils.cpp
    PIDController.cpp
    SignalUtils.cpp
    main.cpp
  tests/
    test_counter.cpp
    test_lowpassfilter.cpp
    test_math_utils.cpp
    test_pid_controller.cpp
    test_signal_utils.cpp
  README.md
```

## 已实现函数

- `clamp(value, min_value, max_value)`：将数值限制在指定范围内。
- `mean(data, size)`：计算数组平均值。
- `max_value(data, size)`：计算数组最大值。
- `range(data, size)`：计算数组极差，即最大值减最小值。
- `normalize_pwm(command)`：将 `-1.0` 到 `1.0` 的控制指令映射到 `-1000` 到 `1000`。
- `deadband(value, threshold)`：对小信号做死区处理，常用于摇杆输入、传感器噪声和电机控制指令预处理。

## 已实现类

### `Counter`

用于练习最小 C++ 类结构。

已实现：

- 默认构造函数
- 带初始值构造函数
- `increment()`
- `reset()`
- `value() const`

这个类主要用于理解对象、成员变量、成员函数、构造函数和 `private` 封装。

### `LowPassFilter`

一阶低通滤波器，用于平滑采样数据。

核心公式：

\[
y_k = \alpha x_k + (1 - \alpha)y_{k-1}
\]

已实现：

- 构造时设置滤波系数 `alpha`
- 使用 `clamp()` 将 `alpha` 限制在 `0.0` 到 `1.0`
- 第一次 `update()` 直接用输入初始化输出
- 后续 `update()` 使用一阶低通公式
- `output()` 读取当前输出
- `reset()` 清空滤波器状态

内部状态：

- `alpha_`：滤波系数
- `previous_output_`：上一次滤波输出
- `initialized_`：是否已经完成第一次初始化

### `PIDController`

PID 控制器模块，用于练习控制器类封装。

已实现：

- P 项：根据当前误差输出
- I 项：按 `error * dt` 累积积分
- D 项：根据误差变化率计算微分
- 输出限幅
- 积分限幅
- `reset()` 状态重置
- `integral()` 读取当前积分项
- `previous_error()` 读取上一次误差
- `dt <= 0.0` 时返回 `0.0`，避免除以 0

内部状态：

- `integral_`：积分项
- `previous_error_`：上一次误差
- `has_previous_error_`：是否已有历史误差
- `output_min_ / output_max_`：输出限幅
- `integral_min_ / integral_max_`：积分限幅

## 编译方法

在项目目录下运行：

```powershell
cmake -S . -B build
cmake --build build
```

如果已经生成过 `build` 目录，平时只需要运行：

```powershell
cmake --build build
```

## 运行主程序

```powershell
.\build\day01_app.exe
```

主程序会演示部分数学工具函数、低通滤波器和 PID 控制器的基本调用。

## 测试方法

运行全部测试：

```powershell
.\build\day01_test.exe
.\build\test_signalutils.exe
.\build\test_counter.exe
.\build\test_LowPassFilter.exe
.\build\test_pid_controller.exe
```

测试程序没有输出表示通过。如果断言失败，程序会中止并报错。

## 测试覆盖内容

### `test_math_utils`

验证：

- `clamp()` 正常值、上限、下限
- `mean()` 正常数组和空数据
- `max_value()` 正数数组、负数数组和空数据
- `normalize_pwm()` 正常映射和超范围限幅
- `range()` 正常数组、负数数组、单元素数组和空数据

### `test_signalutils`

验证：

- 小信号被死区压成 `0.0`
- 正负大信号保持原值
- 负阈值也能按绝对值处理

### `test_counter`

验证：

- 默认构造初始值为 `0`
- 带初始值构造能正确设置初始值
- `increment()` 正确递增
- `reset()` 正确清零

### `test_lowpassfilter`

验证：

- 第一次 `update()` 直接返回输入
- 连续调用时按一阶低通公式更新
- 多个滤波器对象互不影响
- `reset()` 后 `output()` 清零
- `reset()` 后下一次输入重新初始化输出

### `test_pid_controller`

验证：

- P-only 控制器输出正确
- PI 控制器连续调用时积分正确累积
- `integral()` 能读到积分项
- 输出限幅生效
- 积分限幅生效
- D 项第一次调用为 `0.0`
- D 项第二次调用按误差变化率计算
- `dt <= 0.0` 时返回 `0.0`
- `reset()` 清空积分项和上一次误差

## 第一周学习记录

- 学习了 C++ 最小工程结构。
- 理解了 `.h` 放声明，`.cpp` 放实现。
- 学会了用 CMake 编译多个源文件。
- 学会了用 `assert` 写最小测试。
- 理解了数组传入函数时需要额外传入长度。
- 理解了只读输入应使用 `const`。
- 实现了若干控制相关基础函数，包括限幅、均值、最大值、极差、死区和 PWM 映射。

## 第二周学习记录

- 学习了 `class`、对象、成员变量、成员函数和构造函数。
- 理解了 `private` 用于隐藏模块内部状态，`public` 用于暴露稳定接口。
- 理解了普通函数适合无状态计算，而滤波器、PID、状态机等模块需要保存历史状态，更适合用类封装。
- 实现了 `LowPassFilter`，理解了 `previous_output_` 和 `initialized_` 的作用。
- 实现了 `PIDController`，理解了 `integral_`、`previous_error_` 和 `has_previous_error_` 的作用。
- 理解了 `reset()` 对有状态控制模块的重要性。
- 学会了为有状态模块设计连续调用测试、边界测试和状态重置测试。

## 当前阶段检查标准

完成当前项目后，应能独立说明：

- `.h` 和 `.cpp` 分别放什么。
- CMake 如何把库、主程序和测试程序连接起来。
- 为什么数组函数需要传入 `size`。
- 为什么只读数组参数要加 `const`。
- 为什么控制输出要先限幅。
- 为什么低通滤波器需要保存上一次输出。
- 为什么 PID 需要保存积分项和上一次误差。
- 为什么测试有状态模块时不能只调用一次。
- 为什么边界测试能发现正常输入测不出的错误。

## 后续计划

下一阶段建议继续实现：

- `std::array` 固定长度数据练习
- `RingBuffer` 环形缓冲区
- 移动平均滤波器
- 采样数据窗口
- 串口数据包解析器
- 简单状态机

后续重点应继续贴近嵌入式控制场景：固定内存、确定性执行、状态清晰、测试可复现。
