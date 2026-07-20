#include <cassert>
#include "control_basics/CommandParser.h"

void test_known_command(){
    control_basics::CommandParser parser;
    assert(parser.parse("START").command == control_basics::CommandType::Start);
    assert(parser.parse("START").status == control_basics::ParseStatus::Ok);
    assert(parser.parse("STOP").command == control_basics::CommandType::Stop);
    assert(parser.parse("STOP").status == control_basics::ParseStatus::Ok);
    assert(parser.parse("RESET").command == control_basics::CommandType::Reset);
    assert(parser.parse("RESET").status == control_basics::ParseStatus::Ok);
    assert(parser.parse("STATUS").command == control_basics::CommandType::Status);
    assert(parser.parse("STATUS").status == control_basics::ParseStatus::Ok);
}
void test_error_command(){
    control_basics::CommandParser parser;
    assert(parser.parse("").command == control_basics::CommandType::Unknown);
    assert(parser.parse("").status == control_basics::ParseStatus::Empty);
    assert(parser.parse("youyun").command == control_basics::CommandType::Unknown);
    assert(parser.parse("youyun").status == control_basics::ParseStatus::Unknown); 
}
void test_normalized_commands(){
    control_basics::CommandParser parser;
    assert(parser.parse("").command == control_basics::CommandType::Unknown);
    assert(parser.parse("").status == control_basics::ParseStatus::Empty);
    assert(parser.parse(" start ").command == control_basics::CommandType::Start);
    assert(parser.parse(" start ").status == control_basics::ParseStatus::Ok);
    assert(parser.parse(" STOP\r\n ").command == control_basics::CommandType::Stop);
    assert(parser.parse(" STOP\r\n ").status == control_basics::ParseStatus::Ok); 
    assert(parser.parse("   ").status == control_basics::ParseStatus::Empty);
    assert(parser.parse("reset").command == control_basics::CommandType::Reset);
    assert(parser.parse(" Status ").command == control_basics::CommandType::Status);
}

int main(){
    control_basics::ParseResult result;
    result.command = control_basics::CommandType::Start;
    result.status = control_basics::ParseStatus::Ok;

    assert(result.command == control_basics::CommandType::Start);
    assert(result.status == control_basics::ParseStatus::Ok);

    test_known_command();
    test_error_command();
    test_normalized_commands();


    return 0;
}