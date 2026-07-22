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
void test_set_speed_command() {
    control_basics::CommandParser parser;

    const control_basics::ParseResult result = parser.parse("SET_SPEED 120");

    assert(result.command == control_basics::CommandType::SetSpeed);
    assert(result.status == control_basics::ParseStatus::Ok);
    assert(result.argument_count == 1);
    assert(result.arguments[0] == 120.0);

    assert(parser.parse("SET_SPEED").status == control_basics::ParseStatus::TooFewArguments);
    assert(parser.parse("SET_SPEED 120 130").status == control_basics::ParseStatus::TooManyArguments);
    assert(parser.parse("SET_SPEED fast").status == control_basics::ParseStatus::InvalidArgument);
    assert(parser.parse("SET_SPEED 12abc").status == control_basics::ParseStatus::InvalidArgument);
    assert(parser.parse("SET_SPEED -1").status == control_basics::ParseStatus::OutOfRange);
    assert(parser.parse("SET_SPEED 99999").status == control_basics::ParseStatus::OutOfRange);
}

void test_move_command(){
    control_basics::CommandParser parser;

    const control_basics::ParseResult result = parser.parse("MOVE 10 20");

    assert(result.command == control_basics::CommandType::Move);
    assert(result.status == control_basics::ParseStatus::Ok);
    assert(result.argument_count == 2);
    assert(result.arguments[0] == 10.0);
    assert(result.arguments[1] == 20.0);

    assert(parser.parse("MOVE 10").status == control_basics::ParseStatus::TooFewArguments);
    assert(parser.parse("MOVE 10 20 30").status == control_basics::ParseStatus::TooManyArguments);
    assert(parser.parse("MOVE 10 abc").status == control_basics::ParseStatus::InvalidArgument);
    assert(parser.parse("MOVE abc 1").status == control_basics::ParseStatus::InvalidArgument);
    assert(parser.parse("MOVE -1 20").status == control_basics::ParseStatus::OutOfRange);
    assert(parser.parse("MOVE 10 999").status == control_basics::ParseStatus::OutOfRange);
}

void test_no_argument_commands_reject_extra_arguments() {
    control_basics::CommandParser parser;

    assert(parser.parse("START").status == control_basics::ParseStatus::Ok);
    assert(parser.parse("START 123").status == control_basics::ParseStatus::TooManyArguments);

    assert(parser.parse("STOP 123").status == control_basics::ParseStatus::TooManyArguments);
    assert(parser.parse("RESET 123").status == control_basics::ParseStatus::TooManyArguments);
    assert(parser.parse("STATUS now").status == control_basics::ParseStatus::TooManyArguments);

    assert(parser.parse("HOME").command == control_basics::CommandType::Home);
    assert(parser.parse("HOME").status == control_basics::ParseStatus::Ok);
    assert(parser.parse("HOME 1").status == control_basics::ParseStatus::TooManyArguments);
    assert(parser.parse(" home ").command == control_basics::CommandType::Home);
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
    test_set_speed_command();
    test_move_command();
    test_no_argument_commands_reject_extra_arguments();
    
    return 0;
}