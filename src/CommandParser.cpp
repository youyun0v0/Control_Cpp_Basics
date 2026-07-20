#include "control_basics/CommandParser.h"

namespace control_basics
{
    ParseResult CommandParser::parse(const std::string& input) const{
        std::string normalized = CommandParser::normalize(input);
        if(normalized == "START") return {CommandType::Start,ParseStatus::Ok};
        if(normalized == "STOP") return {CommandType::Stop,ParseStatus::Ok};
        if(normalized == "RESET") return {CommandType::Reset,ParseStatus::Ok};
        if(normalized == "STATUS") return {CommandType::Status,ParseStatus::Ok};
        if(normalized.empty() ) return {CommandType::Unknown,ParseStatus::Empty};
        else return {CommandType::Unknown,ParseStatus::Unknown};
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
}