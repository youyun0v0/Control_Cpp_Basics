#include "control_basics/CommandParser.h"
#include <sstream>

namespace control_basics
{
    ParseResult CommandParser::parse(const std::string& input) const {
        const std::string normalized = normalize(input);
        const TokenizedInput tokens = tokenize(normalized);

        if (tokens.count == 0) {
            return {CommandType::Unknown, ParseStatus::Empty, {}, 0};
        }//空指令

        const std::string& command = tokens.tokens[0]; //分析指令
        //无参数指令
        if (command == "START") {
            return parse_no_argument_command(tokens, CommandType::Start);
        }
        if (command == "STOP") {
            return parse_no_argument_command(tokens, CommandType::Stop);
        }
        if (command == "RESET") {
            return parse_no_argument_command(tokens, CommandType::Reset);
        }
        if (command == "STATUS") {
            return parse_no_argument_command(tokens, CommandType::Status);
        }
        if (command == "HOME") {
            return parse_no_argument_command(tokens, CommandType::Home);
        }
        //有参数指令
        if (command == "SET_SPEED") {
            return parse_set_speed(tokens);
        }
        if( command == "MOVE") {
            return parse_move(tokens);
        }
        //未知指令
        return {CommandType::Unknown, ParseStatus::Unknown, {}, 0};
    }

    std::string CommandParser::normalize(const std::string& input) const {
        std::size_t start = 0;
        while (start < input.size() &&
               std::isspace(static_cast<unsigned char>(input[start])) != 0) {
        ++start;
        }

        std::size_t end = input.size();
        while (end > start &&
               std::isspace(static_cast<unsigned char>(input[end - 1])) != 0) {
        --end;
        }

        std::string output;
        for (std::size_t i = start; i < end; ++i) {
            output.push_back( static_cast<char>(std::toupper(static_cast<unsigned char>(input[i]))) );
        }

        return output;
    }

    CommandParser::TokenizedInput CommandParser::tokenize(const std::string& input) const {
        TokenizedInput result;
        std::istringstream stream(input);
        std::string token;

        while(stream >> token){ //从stream中读入一个词（分隔符分割），传入token
            if(result.count < result.tokens.size()){
                result.tokens[result.count] = token;
            }

            result.count ++;
        }
        return result;
    }

    ParseResult CommandParser::parse_set_speed(const CommandParser::TokenizedInput& input) const{
        if(input.count < 2){
            return {CommandType::SetSpeed, ParseStatus::TooFewArguments, {}, 0};
        }
        if(input.count > 2){
            return {CommandType::SetSpeed, ParseStatus::TooManyArguments, {}, 0};
        }
        double speed = 0.0;
        if(!parse_double_token(input.tokens[1], speed)){
            return {CommandType::SetSpeed, ParseStatus::InvalidArgument, {}, 0};
        }
        if(speed < 0.0 || speed > 3000.0){
            return {CommandType::SetSpeed, ParseStatus::OutOfRange, {}, 0};
        }
        return {CommandType::SetSpeed, ParseStatus::Ok, {speed, 0.0}, 1};
    }

    ParseResult CommandParser::parse_move(const CommandParser::TokenizedInput& input) const{
        if(input.count < 3){
            return {CommandType::Move, ParseStatus::TooFewArguments, {}, 0};
        }
        if(input.count > 3){
            return {CommandType::Move, ParseStatus::TooManyArguments, {}, 0};
        }
        double x = 0.0, y = 0.0;
        if(!parse_double_token(input.tokens[1], x) || !parse_double_token(input.tokens[2], y)){
            return {CommandType::Move, ParseStatus::InvalidArgument, {}, 0};
        }
        if(x < 0.0 || x > 200.0 || y < 0.0 || y > 200.0){
            return {CommandType::Move, ParseStatus::OutOfRange, {}, 0};
        }
        return {CommandType::Move, ParseStatus::Ok, {x, y}, 2};
    }

    ParseResult CommandParser::parse_no_argument_command(const TokenizedInput& input, CommandType command) const{
        if(input.count > 1){
            return {command, ParseStatus::TooManyArguments, {}, 0};
        }
        return {command, ParseStatus::Ok, {}, 0};
    }

    bool CommandParser::parse_double_token(const std::string& token, double& value) const {
        std::istringstream stream(token);
        stream >> value;
        if(stream.fail()) return false;
        char extra = '\0';
        if(stream >> extra) return false; //数字后面可能还有垃圾字符
        return true;
    }
}