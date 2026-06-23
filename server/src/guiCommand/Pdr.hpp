/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Pdr
*/

#pragma once
#include <cstddef>
#include <string>

#include "IGuiCommand.hpp"

namespace zappy::server::guiCommand {
class Pdr : public IGuiCommand {
  public:
    explicit Pdr(std::size_t id, std::size_t resourceId);
    Pdr(const Pdr& other) = delete;
    Pdr& operator=(const Pdr& other) = delete;
    Pdr(Pdr&& other) = delete;
    Pdr& operator=(Pdr&& other) = delete;
    ~Pdr() override = default;

    std::string execute(Core& core) override;

  private:
    std::size_t _resourceId;
    std::size_t _id;
};
}  // namespace zappy::server::guiCommand
