/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Command factory
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "ICommand.hpp"

namespace zappy::server::command {

class CommandFactory {
  public:
    CommandFactory();
    CommandFactory(const CommandFactory& other) = delete;
    CommandFactory& operator=(const CommandFactory& other) = delete;
    CommandFactory(CommandFactory&& other) = delete;
    CommandFactory& operator=(CommandFactory&& other) = delete;
    ~CommandFactory() = default;

    [[nodiscard]] std::unique_ptr<ICommand> createCommand(std::string_view rawCommand) const;

  private:
    using CommandCreator = std::move_only_function<std::unique_ptr<ICommand>(std::string_view) const>;
    std::unordered_map<std::string, CommandCreator> _creators;

    [[nodiscard]] static std::string extractFirstWord(std::string_view rawCommand);
};

}  // namespace zappy::server::command
