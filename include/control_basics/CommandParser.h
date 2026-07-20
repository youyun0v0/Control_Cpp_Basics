#pragma once
#include <string>
#include <algorithm>
#include <cctype>

namespace control_basics {

enum class CommandType {
    Start,
    Stop,
    Reset,
    Status,
    Unknown
};//指令类型
enum class ParseStatus {
    Ok,
    Empty,
    Unknown
};//分析器状态

struct ParseResult {
    CommandType command;
    ParseStatus status;
};//分析结果

class CommandParser {
public:
    ParseResult parse(const std::string& input) const;//分析：把字符串变成指令

private:
    std::string normalize(const std::string& input) const;//自带对字符串的标准化函数
};

}  // namespace control_basics