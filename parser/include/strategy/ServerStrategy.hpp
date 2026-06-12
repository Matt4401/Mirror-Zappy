/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ServerStrategy
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "IParseStrategy.hpp"
#include "exception/ParsingError.hpp"

namespace zappy::parser {
struct ServerConfig {
    std::uint16_t port{};
    std::uint32_t width{};
    std::uint32_t height{};
    std::vector<std::string> teamNames;
    std::uint32_t clientLimit{};
    std::uint32_t freq = 100;
};

class ServerStrategy : public IParseStrategy<ServerConfig> {
  public:
    void parse(int argc, char** argv, ServerConfig& config) override;

  private:
    static constexpr std::string kServerConfigFlags = "p:x:y:c:f:n:";
    static constexpr std::string kReservedGraphicName = "GRAPHIC";

    static int parseNumericArg(const std::string& arg, const std::string& flagName);
    static void processOptions(int argc, char** argv, ServerConfig& config);
    static void validate(const ServerConfig& config);
    static bool handleUsage(char** argv, int argc);

    template <typename T>
    static void isValidate(T value, const std::string& error) {
        if (value <= 0) {
            throw shared::exception::ParsingError(error);
        }
    }
};
}  // namespace zappy::parser
