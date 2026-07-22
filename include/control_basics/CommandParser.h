#pragma once
#include <string>
#include <algorithm>
#include <cctype>
#include <array>
#include <cstddef>

namespace control_basics {

enum class CommandType {
    Start,
    Stop,
    Reset,
    Status,
    SetSpeed,
    Move,
    Home,
    Unknown
};//指令类型
enum class ParseStatus {
    Ok,
    Empty,
    TooFewArguments,
    TooManyArguments,
    InvalidArgument,
    OutOfRange,
    Unknown
};//分析器状态

struct ParseResult {
    CommandType command = CommandType::Unknown;
    ParseStatus status = ParseStatus::Unknown;
    std::array<double, 2> arguments{};
    std::size_t argument_count = 0;
};//分析结果
//结构体赋初值，方便之后进行部分定义，不用引出全部成员

class CommandParser {
public:
    ParseResult parse(const std::string& input) const;//分析：把字符串变成指令
    
private:
    struct TokenizedInput {
        std::array<std::string, 4> tokens{};
        std::size_t count = 0;
    };//把输入进行分词

    std::string normalize(const std::string& input) const;//自带对字符串的标准化函数
    TokenizedInput tokenize(const std::string& input) const;//自带分词函数
    bool parse_double_token(const std::string& token, double& value) const;//把分词转为数字

    ParseResult parse_set_speed(const TokenizedInput & input) const;//分析：SET_SPEED指令
    ParseResult parse_move(const TokenizedInput& input) const;//分析：move指令
    ParseResult parse_no_argument_command(const TokenizedInput& input, CommandType command) const;
};

}  // namespace control_basics