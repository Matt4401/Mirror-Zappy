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
#include <vector>

#include "ICommand.hpp"
#include "guiCommand/IGuiCommand.hpp"
#include "protocol/Commands.hpp"

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
    [[nodiscard]] std::unique_ptr<guiCommand::IGuiCommand> createGuiCommand(std::string_view rawCommand) const;

  private:
    using CommandCreator = std::move_only_function<std::unique_ptr<ICommand>(std::string_view) const>;
    using GuiCommandCreator = std::move_only_function<std::unique_ptr<guiCommand::IGuiCommand>(std::string_view) const>;

    void registerCommands();
    void registerGuiCommands();
    template <typename ParsedCommandType, typename GuiCommandType>
    static std::unique_ptr<guiCommand::IGuiCommand> parseAndCreateGuiCommand(std::string_view rawCommand);

    std::unordered_map<std::string, CommandCreator> _creators;
    std::unordered_map<std::string, GuiCommandCreator> _guiCreators;

    [[nodiscard]] static std::string extractFirstWord(std::string_view rawCommand);
    static std::vector<std::string> extractAllCmd(std::string_view rawCommand);
};

}  // namespace zappy::server::command
