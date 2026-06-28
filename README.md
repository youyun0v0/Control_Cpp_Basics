# control_cpp_basics

这个项目用于通过小型控制相关函数重建 C++ 工程基础。

## 文件结构

```text
control_cpp_basics/
  CMakeLists.txt
  include/control_basics/
  src/
  tests/
  README.md
```

## 已实现函数

- `clamp(value, min_value, max_value)`：将数值限制在指定范围内。
- `mean(data, size)`：计算数组平均值。
- `max_value(data, size)`：计算数组最大值。
- `deadband(value, threshold)`：对小信号做死区处理。
- `normalize_pwm(command)`：将 `-1.0` 到 `1.0` 的控制指令映射到 `-1000` 到 `1000`。

## 编译方法

```powershell
cmake -S . -B build
cmake --build build
```

## 运行方法

```powershell
.\build\day01_app.exe
``` 

## 测试方法

```powershell
.\build\day01_test.exe
.\build\test_signalutils.exe
```

测试程序没有输出表示通过。如果断言失败，程序会报错。

## 本周学习记录

- 本周学习了 C++ 最小工程结构。
- `.h` 放函数声明，`.cpp` 放函数实现。
- 数组传入函数时需要额外传入长度。只读输入应使用 `const`。
- 控制输出应先限幅再映射到底层执行器范围。每个函数都应写测试验证正常情况和边界情况。