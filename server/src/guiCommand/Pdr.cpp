/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Pdr
*/

#include "Pdr.hpp"

#include <cstddef>
#include <string>

#include "IGuiCommand.hpp"

namespace zappy::server::guiCommand {

Pdr::Pdr(const std::size_t id, const std::size_t resourceId) : _resourceId{resourceId}, _id{id} {}

std::string Pdr::execute(Core& core) {}

}  // namespace zappy::server::guiCommand
