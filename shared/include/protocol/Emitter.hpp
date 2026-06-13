/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** protocol emitter interface
*/

#pragma once

#include <string>

#include "protocol/Commands.hpp"

namespace zappy::shared::protocol {

class Emitter {
  public:
    Emitter() = default;
    ~Emitter() = default;

    Emitter(const Emitter& other) = delete;
    Emitter& operator=(const Emitter& other) = delete;
    Emitter(Emitter&& other) = delete;
    Emitter& operator=(Emitter&& other) = delete;

    /**
     * @brief Build a raw protocol string from a ServerCommand.
     */
    [[nodiscard]] static std::string build(const ServerCommand& cmd);

    /**
     * @brief Build a raw protocol string from a ClientCommand.
     */
    [[nodiscard]] static std::string build(const ClientCommand& cmd);
};

}  // namespace zappy::shared::protocol
