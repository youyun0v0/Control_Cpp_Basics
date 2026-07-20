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
      CommandExecutor.h
      CommandParser.h
      CommandQueue.h
      Counter.h
      LowPassFilter.h
      MathUtils.h
      MotionStateMachine.h
      MovingAverageFilter.h
      PIDController.h
      RingBuffer.h
      SignalUtils.h
  src/
    CommandExecutor.cpp
    CommandParser.cpp
    CommandQueue.cpp
    Counter.cpp
    LowPassFilter.cpp
    MathUtils.cpp
    MotionStateMachine.cpp
    MovingAverageFilter.cpp
    PIDController.cpp
    RingBuffer.cpp
    SignalUtils.cpp
    main.cpp
  tests/
    test_array_basics.cpp
    test_command_executor.cpp
    test_command_flow.cpp
    test_command_parser.cpp
    test_command_queue.cpp
    test_counter.cpp
    test_lowpassfilter.cpp
    test_math_utils.cpp
    test_motion_state_machine.cpp
    test_moving_average_filter.cpp
    test_pid_controller.cpp
    test_ring_buffer.cpp
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

### `RingBuffer`

固定容量环形缓冲区，用于保存最近一段采样数据。

已实现：

- `push(value)`：写入一个新数据，写满后覆盖最旧数据
- `clear()`：清空缓冲区状态
- `count()`：读取当前有效数据数量
- `capacity()`：读取固定容量
- `empty()`：判断是否为空
- `full()`：判断是否已满
- `latest()`：读取最近一次写入的数据
- `at(index)`：按从旧到新的时间顺序读取第 `index` 个有效数据

内部状态：

- `data_`：固定容量数组
- `write_index_`：下一次写入的位置
- `count_`：当前有效数据数量

### `MovingAverageFilter`

移动平均滤波器，用于对最近一段采样数据求平均。

已实现：

- `update(input)`：写入新采样并返回当前移动平均值
- `output()`：读取当前输出
- `reset()`：清空内部缓冲区和输出
- `count()`：读取当前参与平均的有效数据数量

内部状态：

- `buffer_`：保存最近采样值的 `RingBuffer`
- `output_`：当前移动平均输出

### `MotionStateMachine`

简化运动设备状态机，用于练习设备状态管理、错误处理和急停逻辑。

状态枚举：

- `Idle`：空闲，允许启动
- `Running`：正在运行，不允许重复启动
- `Stopped`：正常停止，允许再次启动
- `Error`：错误状态，不允许直接启动

错误码枚举：

- `None`：无错误
- `LimitTriggered`：限位触发
- `SensorInvalid`：传感器数据无效
- `EmergencyStop`：急停

已实现：

- `start()`：在 `Idle` 或 `Stopped` 状态下启动，进入 `Running`
- `stop()`：在 `Running` 状态下正常停止，进入 `Stopped`
- `set_error(error)`：进入 `Error` 状态并记录错误码
- `reset_error()`：从 `Error` 状态恢复到 `Idle`，并清空错误码
- `emergency_stop()`：急停，等价于 `set_error(ErrorCode::EmergencyStop)`
- `can_start()`：判断当前是否允许启动
- `is_error()`：判断当前是否处于错误状态
- `state()`：读取当前状态
- `error()`：读取当前错误码

内部状态：

- `state_`：当前设备状态
- `error_`：当前错误码

### `CommandParser`

串口命令解析器，用于把外部输入的字符串命令转换成程序内部的命令类型。

命令枚举：

- `Start`：启动命令
- `Stop`：停止命令
- `Reset`：错误复位命令
- `Status`：状态查询命令
- `Unknown`：未知命令

解析状态枚举：

- `Ok`：解析成功
- `Empty`：输入为空或标准化后为空
- `Unknown`：无法识别的命令

已实现：

- `parse(input)`：解析字符串并返回 `ParseResult`
- `normalize(input)`：整理输入，去掉前后空白，并把字母统一转成大写

内部规则：

- `"START"`、`" start "`、`"start"` 都会解析为 `CommandType::Start`
- `"STOP\r\n"` 会解析为 `CommandType::Stop`
- 空字符串或只有空白字符的输入会返回 `ParseStatus::Empty`
- 未知输入会返回 `ParseStatus::Unknown`

### `CommandExecutor`

命令执行器，用于把已经解析好的命令真正执行到 `MotionStateMachine`。

执行状态枚举：

- `Ok`：命令执行成功
- `Rejected`：命令合法，但当前状态机状态不允许执行
- `InvalidCommand`：无效命令

已实现：

- `execute(command, machine)`：根据命令调用状态机动作，并返回 `ExecuteResult`

命令和状态机动作的对应关系：

- `CommandType::Start` 调用 `machine.start()`
- `CommandType::Stop` 调用 `machine.stop()`
- `CommandType::Reset` 调用 `machine.reset_error()`
- `CommandType::Status` 只读取当前状态，不改变状态机
- `CommandType::Unknown` 返回 `InvalidCommand`

### `CommandQueue`

固定容量命令队列，用于暂存已经解析成功、等待执行的命令。

已实现：

- `push(command)`：将命令放入队列
- `pop(command)`：按先进先出顺序取出命令
- `clear()`：清空队列
- `count()`：读取当前命令数量
- `capacity()`：读取固定容量
- `empty()`：判断队列是否为空
- `full()`：判断队列是否已满

内部状态：

- `data_`：固定容量命令数组
- `head_`：下一次读取的位置
- `tail_`：下一次写入的位置
- `count_`：当前队列中的命令数量

核心规则：

- 命令队列按先进先出顺序工作
- 队列满时拒绝继续写入
- 未知命令不应进入正常执行流程

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
.\build\main_test.exe
```

主程序会演示部分数学工具函数、低通滤波器、PID 控制器、移动平均滤波器和状态机的基本调用。

## 测试方法

运行全部测试：

```powershell
.\build\test_math_utils.exe
.\build\test_signalutils.exe
.\build\test_counter.exe
.\build\test_LowPassFilter.exe
.\build\test_pid_controller.exe
.\build\test_array_basics.exe
.\build\test_ring_buffer.exe
.\build\test_moving_average_filter.exe
.\build\test_motion_state_machine.exe
.\build\test_command_parser.exe
.\build\test_command_executor.exe
.\build\test_command_queue.exe
.\build\test_command_flow.exe
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

### `test_array_basics`

验证：

- `std::array` 固定长度数组能保存采样数据
- 模板函数能接收不同长度的 `std::array`
- 平均值计算结果正确

### `test_ring_buffer`

验证：

- 初始状态为空，容量为 `8`
- 写入单个数据后 `count()` 和 `latest()` 正确
- 刚好写满后，`at()` 能按从旧到新的顺序读出 `1.0` 到 `8.0`
- 写满后继续写入时，最旧数据被覆盖
- 越界读取返回 `0.0`
- `clear()` 后回到空状态

### `test_moving_average_filter`

验证：

- 初始输出为 `0.0`
- 窗口未满时，按当前有效数据数量求平均
- 写满 8 个数据后，平均值正确
- 超过容量后，只对最近 8 个数据求平均
- `reset()` 后 `count()` 为 `0`，`output()` 为 `0.0`

### `test_motion_state_machine`

验证：

- 初始状态为 `Idle`
- 初始状态允许启动
- `start()` 后进入 `Running`
- `Running` 状态下不允许重复启动
- `stop()` 后进入 `Stopped`
- `Stopped` 状态允许再次启动
- `Idle` 状态下 `stop()` 失败，并保持 `Idle`
- `set_error()` 后进入 `Error`
- `Error` 状态下 `start()` 失败
- `emergency_stop()` 后错误码为 `EmergencyStop`
- `reset_error()` 后恢复到 `Idle`

### `test_command_parser`

验证：

- `START / STOP / RESET / STATUS` 能解析成对应 `CommandType`
- 解析成功时返回 `ParseStatus::Ok`
- 空字符串返回 `ParseStatus::Empty`
- 未知字符串返回 `ParseStatus::Unknown`
- 带前后空格的命令可以解析
- 带 `\r\n` 的命令可以解析
- 小写或混合大小写命令可以解析
- 只有空白字符的输入返回 `ParseStatus::Empty`

### `test_command_executor`

验证：

- `Start` 命令能让空闲状态机进入 `Running`
- `Stop` 命令能让运行状态机进入 `Stopped`
- `Reset` 命令能让错误状态机恢复到 `Idle`
- `Status` 命令只读取状态，不改变状态机
- `Unknown` 命令返回 `InvalidCommand`
- 错误状态下执行 `Start` 会被拒绝，并保持错误状态

### `test_command_queue`

验证：

- 新建队列为空
- `push()` 后命令数量增加
- `pop()` 按先进先出顺序取出命令
- 空队列 `pop()` 失败
- 队列写满后 `full()` 为 `true`
- 满队列继续 `push()` 失败
- `clear()` 后队列恢复为空

### `test_command_flow`

验证：

- 单条 `"START"` 可以经过解析、入队、出队、执行后让状态机进入 `Running`
- 未知输入解析失败后不进入队列，队列保持为空
- 多条输入 `" start\r\n"`、`" StATus"`、`" STOP  "` 能按顺序执行
- `Status` 命令不会改变状态机状态
- 完整流程最终可以让状态机从 `Idle` 进入 `Running`，再进入 `Stopped`

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

## 第三周学习记录

- 学习了 `std::array` 固定长度数组，理解了固定容量数据结构更适合嵌入式控制场景。
- 实现了 `RingBuffer` 环形缓冲区，用固定容量数组保存最近 8 个采样值。
- 理解了 `write_index_` 表示下一次写入位置，不是最近写入位置。
- 理解了缓冲区写满后，`write_index_` 指向当前最旧数据的位置；`latest()` 读取的是 `write_index_` 的前一位。
- 实现了 `at(index)`，按从旧到新的时间顺序读取有效数据。
- 学会了测试空缓冲区、未满、刚满、超过容量、越界读取和 `clear()` 后状态。
- 实现了 `MovingAverageFilter`，基于 `RingBuffer` 对最近采样值求平均。
- 理解了移动平均可以平滑采样数据，但会受到历史数据影响，因此会引入延迟。
- 理解了测试环形缓冲区时不能使用一堆相同值，否则会掩盖顺序错误。

## 第四周学习记录

- 学习了 `enum class`，用它表示有限状态和错误码。
- 实现了 `MotionStateMachine`，理解了状态机用于约束设备行为。
- 理解了状态和动作的区别：`Idle / Running / Stopped / Error` 是状态，`start()`、`stop()`、`emergency_stop()` 是动作。
- 理解了状态切换规则：不是任何动作在任何状态下都允许执行。
- 学会了用 `bool` 返回值表示动作是否成功。
- 加入了错误码 `ErrorCode`，能记录进入错误状态的原因。
- 理解了普通停止和急停的区别：普通停止进入 `Stopped`，急停进入 `Error`。
- 学会了测试合法切换、非法切换、错误阻塞启动和错误复位。

## 第五周学习记录

- 学习了把外部字符串命令解析成程序内部命令枚举。
- 实现了 `CommandParser`，支持 `START / STOP / RESET / STATUS`。
- 理解了真实输入可能带空格、换行和大小写差异，因此需要先做输入标准化。
- 理解了 `ParseStatus` 表示字符串是否解析成功，`CommandType` 表示解析出的命令类型。
- 实现了 `CommandExecutor`，把 `CommandType` 连接到 `MotionStateMachine` 的动作函数。
- 理解了 `ExecuteStatus` 表示命令是否执行成功，和设备本身的 `MotionState` 不是同一件事。
- 理解了 `Parser` 只负责看懂字符串，`Executor` 只负责执行命令，`MotionStateMachine` 负责判断动作能不能生效。
- 实现了固定容量 `CommandQueue`，按先进先出顺序保存待执行命令。
- 理解了命令队列和采样环形缓冲区的区别：采样数据可以覆盖旧值，未执行命令不能随便覆盖。
- 学会了写从原始输入到状态机执行结果的完整流程测试。

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
- 为什么环形缓冲区写满后，内部数组顺序不一定等于时间顺序。
- 为什么 `at(0)` 表示最旧的有效数据，而 `latest()` 表示最近写入的数据。
- 为什么移动平均窗口未满时，分母应使用有效数据数量。
- 为什么移动平均会让数据更平滑，同时也引入响应延迟。
- 为什么状态机不能让外部直接修改 `state_`。
- 为什么 `start()` 和 `stop()` 要根据当前状态决定是否成功。
- 为什么错误状态下不能直接启动。
- 为什么急停应进入 `Error`，而不是普通 `Stopped`。
- 为什么测试状态机时要同时测试合法切换和非法切换。
- 为什么外部字符串命令不能直接修改状态机。
- 为什么要先把字符串解析成 `CommandType`，再执行命令。
- 为什么 `Parser` 和 `Executor` 要分开。
- 为什么 `ParseStatus`、`CommandType`、`ExecuteStatus`、`MotionState` 不是同一类概念。
- 为什么 `Status` 命令只查询状态，不应该改变状态机。
- 为什么命令队列应按先进先出顺序执行。
- 为什么解析失败的命令不应该进入正常执行流程。
- 如何说明一条输入命令从字符串到状态机动作的完整路径。

## 后续计划

下一阶段建议继续实现带参数命令：

- `SET_SPEED 120`
- `MOVE 10 20`
- 参数数量检查
- 参数范围检查
- 字符串转数字
- 命令错误信息
- 更接近 G-code 子集的命令格式

后续重点应继续贴近嵌入式控制场景：固定内存、确定性执行、状态清晰、命令入口清楚、输入合法性明确、测试可复现。
