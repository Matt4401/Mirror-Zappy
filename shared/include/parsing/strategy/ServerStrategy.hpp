/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ServerStrategy
*/

#pragma once

#include <string>

#include "parsing/IParseStrategy.hpp"

namespace zappy::server::util {
struct ServerConfig;
}

namespace zappy::shared::parsing {
class ServerStrategy : public IParseStrategy<server::util::ServerConfig> {
  public:
    void parse(int argc, char** argv, server::util::ServerConfig& config) override;

  private:
    static constexpr std::string kServerConfigFlags = "p:x:y:c:f:n:";
    static constexpr std::string kReservedGraphicName = "GRAPHIC";

    static void validate(const server::util::ServerConfig& config);
    static bool printUsage(char** argv, int argc);
};
}  // namespace zappy::shared::parsing
