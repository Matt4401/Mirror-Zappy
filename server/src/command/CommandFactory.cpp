/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** command factory
*/

#include "CommandFactory.hpp"

#include <memory>
#include <sstream>
#include <string>
#include <string_view>

#include "command/Forward.hpp"
#include "command/ICommand.hpp"

namespace zappy::server::command {

CommandFactory::CommandFactory() {
    _creators.emplace("Forward", [](std::string_view) { return std::make_unique<Forward>(); });
}

std::unique_ptr<ICommand> CommandFactory::createCommand(std::string_view rawCommand) const {
    const std::string firstWord = extractFirstWord(rawCommand);
    auto it = _creators.find(firstWord);

    if (it != _creators.end()) {
        return it->second(rawCommand);
    }
    return nullptr;
}

std::string CommandFactory::extractFirstWord(std::string_view rawCommand) {
    std::istringstream stream(std::string{rawCommand});
    std::string word;

    stream >> word;
    return word;
}

}  // namespace zappy::server::command
