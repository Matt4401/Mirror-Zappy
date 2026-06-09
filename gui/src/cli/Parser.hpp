/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Parser
*/

#pragma once
#include <cstdint>
#include <limits>
#include <span>
#include <string>
#include <vector>

namespace zappy::cli {
class Parser {
  public:
    static constexpr std::int32_t kPortMax = std::numeric_limits<std::uint16_t>::max();

    Parser(std::span<char*> args) : _args(args.subspan(1).begin(), args.subspan(1).end()) {};
    ~Parser() = default;
    Parser(const Parser& other) = delete;
    Parser& operator=(const Parser& other) = delete;
    Parser(Parser&& other) = delete;
    Parser& operator=(Parser&& other) = delete;

    bool parse();

  protected:
  private:
    void parseArgs();
    static std::uint16_t getPort(const std::string& value);
    static std::string getMachine(const std::string& value);
    static std::string printHelp();
    std::vector<std::string> _args;
    std::uint16_t _port{0};
    std::string _machine{"127.0.0.1"};
};
}  // namespace zappy::cli
