/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Parser tests
*/

#include "cli/Parser.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <initializer_list>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "exception/ParserError.hpp"

namespace zappy::cli::tests {
namespace {

struct Arguments {
    std::vector<std::string> storage;
    std::vector<char*> argv;
};

Arguments makeArguments(std::initializer_list<std::string_view> values) {
    Arguments arguments;

    arguments.storage.reserve(values.size());
    arguments.argv.reserve(values.size());
    for (const std::string_view value : values) {
        arguments.storage.emplace_back(value);
    }
    for (std::string& value : arguments.storage) {
        arguments.argv.push_back(value.data());
    }
    return arguments;
}

Parser makeParser(Arguments& arguments) {
    return Parser{std::span<char*>{arguments.argv.data(), arguments.argv.size()}};
}

void expectParserErrorContains(std::initializer_list<std::string_view> values, std::string_view expectedMessage) {
    Arguments arguments = makeArguments(values);
    Parser parser = makeParser(arguments);

    try {
        parser.parse();
        FAIL() << "Expected ParserError";
    } catch (const shared::exception::ParserError& error) {
        EXPECT_THAT(std::string{error.what()}, testing::HasSubstr(std::string{expectedMessage}));
    }
}

}  // namespace

TEST(ParserTest, ParsesValidPortAndMachine) {
    Arguments arguments = makeArguments({"./zappy_gui", "-p", "4242", "-h", "localhost"});
    Parser parser = makeParser(arguments);

    EXPECT_TRUE(parser.parse());
}

TEST(ParserTest, AcceptsHelpArgument) {
    Arguments arguments = makeArguments({"./zappy_gui", "--help"});
    Parser parser = makeParser(arguments);

    testing::internal::CaptureStdout();
    const bool result = parser.parse();
    const std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(result);
    EXPECT_THAT(output, testing::HasSubstr("USAGE: ./zappy_gui -p port -h machine"));
}

TEST(ParserTest, RejectsAlphabeticPort) {
    expectParserErrorContains({"./zappy_gui", "-p", "abc", "-h", "localhost"}, "Invalid port");
}

TEST(ParserTest, RejectsPartiallyNumericPort) {
    expectParserErrorContains({"./zappy_gui", "-p", "12x", "-h", "localhost"}, "must be between 1 and");
}

TEST(ParserTest, RejectsZeroPort) {
    expectParserErrorContains({"./zappy_gui", "-p", "0", "-h", "localhost"}, "must be between 1 and");
}

TEST(ParserTest, RejectsPortAboveUint16Range) {
    expectParserErrorContains({"./zappy_gui", "-p", "70000", "-h", "localhost"}, "must be between 1 and");
}

TEST(ParserTest, RejectsMissingPortValue) {
    expectParserErrorContains({"./zappy_gui", "-h", "localhost", "-p"}, "USAGE: ./zappy_gui -p port -h machine");
}

TEST(ParserTest, RejectsMissingMachineValue) {
    expectParserErrorContains({"./zappy_gui", "-p", "4242", "-h"}, "USAGE: ./zappy_gui -p port -h machine");
}

TEST(ParserTest, RejectsUnknownArgument) {
    expectParserErrorContains({"./zappy_gui", "-p", "4242", "-x", "localhost"}, "Unknown argument: -x");
}

}  // namespace zappy::cli::tests
