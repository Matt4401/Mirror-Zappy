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
#include <variant>
#include <vector>

#include "Eject.hpp"
#include "Inventory.hpp"
#include "Look.hpp"
#include "Set.hpp"
#include "Take.hpp"
#include "command/Broadcast.hpp"
#include "command/Fork.hpp"
#include "command/Forward.hpp"
#include "command/ICommand.hpp"
#include "command/Left.hpp"
#include "command/Right.hpp"
#include "guiCommand/Bct.hpp"
#include "guiCommand/IGuiCommand.hpp"
#include "guiCommand/Mct.hpp"
#include "guiCommand/Msz.hpp"
#include "guiCommand/Pin.hpp"
#include "guiCommand/Sbp.hpp"
#include "guiCommand/Sgt.hpp"
#include "guiCommand/Sst.hpp"
#include "guiCommand/Tna.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Parser.hpp"

namespace zappy::server::command {

CommandFactory::CommandFactory() {
    registerCommands();
    registerGuiCommands();
}

void CommandFactory::registerCommands() {
    _creators.emplace("Broadcast", [](const std::string_view rawCommand) -> std::unique_ptr<ICommand> {
        const auto& parseCmd = extractAllCmd(rawCommand);

        if (parseCmd.size() >= 2) {
            return std::make_unique<Broadcast>(std::string(rawCommand.substr(parseCmd.at(0).size() + 1)));
        }
        return nullptr;
    });
    _creators.emplace("Forward", [](std::string_view) { return std::make_unique<Forward>(); });
    _creators.emplace("Left", [](std::string_view) { return std::make_unique<Left>(); });
    _creators.emplace("Right", [](std::string_view) { return std::make_unique<Right>(); });
    _creators.emplace("Eject", [](std::string_view) { return std::make_unique<Eject>(); });
    _creators.emplace("Inventory", [](std::string_view) { return std::make_unique<Inventory>(); });
    _creators.emplace("Take", [](const std::string_view rawCommand) -> std::unique_ptr<ICommand> {
        const auto& parseCmd = extractAllCmd(rawCommand);

        if (parseCmd.size() == 2) {
            return std::make_unique<Take>(std::string(parseCmd.at(1)));
        }
        return nullptr;
    });
    _creators.emplace("Set", [](const std::string_view rawCommand) -> std::unique_ptr<ICommand> {
        const auto& parseCmd = extractAllCmd(rawCommand);

        if (parseCmd.size() == 2) {
            return std::make_unique<Set>(std::string(parseCmd.at(1)));
        }
        return nullptr;
    });
    _creators.emplace("Fork", [](std::string_view) { return std::make_unique<Fork>(); });
    _creators.emplace("Look", [](std::string_view) { return std::make_unique<Look>(); });
}

void CommandFactory::registerGuiCommands() {
    _guiCreators.emplace("msz", [](std::string_view rawCommand) {
        return parseAndCreateGuiCommand<shared::protocol::client::Msz, guiCommand::Msz>(rawCommand);
    });
    _guiCreators.emplace("sgt", [](std::string_view rawCommand) {
        return parseAndCreateGuiCommand<shared::protocol::client::Sgt, guiCommand::Sgt>(rawCommand);
    });
    _guiCreators.emplace("bct", [](std::string_view rawCommand) {
        return parseAndCreateGuiCommand<shared::protocol::client::Bct, guiCommand::Bct>(rawCommand);
    });
    _guiCreators.emplace("mct", [](std::string_view rawCommand) {
        return parseAndCreateGuiCommand<shared::protocol::client::Mct, guiCommand::Mct>(rawCommand);
    });
    _guiCreators.emplace("tna", [](std::string_view rawCommand) {
        return parseAndCreateGuiCommand<shared::protocol::client::Tna, guiCommand::Tna>(rawCommand);
    });
    _guiCreators.emplace("sst", [](std::string_view rawCommand) {
        return parseAndCreateGuiCommand<shared::protocol::client::Sst, guiCommand::Sst>(rawCommand);
    });
    _guiCreators.emplace("pin", [](std::string_view rawCommand) -> std::unique_ptr<guiCommand::IGuiCommand> {
        return parseAndCreateGuiCommand<shared::protocol::client::Pin, guiCommand::Pin>(rawCommand);
    });
}

std::unique_ptr<ICommand> CommandFactory::createCommand(std::string_view rawCommand) const {
    const std::string firstWord = extractFirstWord(rawCommand);
    auto it = _creators.find(firstWord);

    if (it == _creators.end()) {
        return nullptr;
    }
    return it->second(rawCommand);
}

std::unique_ptr<guiCommand::IGuiCommand> CommandFactory::createGuiCommand(std::string_view rawCommand) const {
    const std::string firstWord = extractFirstWord(rawCommand);
    auto it = _guiCreators.find(firstWord);

    if (it == _guiCreators.end()) {
        return nullptr;
    }
    return it->second(rawCommand);
}

std::string CommandFactory::extractFirstWord(const std::string_view rawCommand) {
    std::istringstream stream(std::string{rawCommand});
    std::string word;

    stream >> word;
    return word;
}

std::vector<std::string> CommandFactory::extractAllCmd(const std::string_view rawCommand) {
    std::istringstream stream(std::string{rawCommand});
    std::string word;
    std::vector<std::string> result;

    while (stream >> word) {
        result.push_back(word);
    }
    return result;
}

template <typename ParsedCommandType, typename GuiCommandType>
std::unique_ptr<guiCommand::IGuiCommand> CommandFactory::parseAndCreateGuiCommand(std::string_view rawCommand) {
    auto parsedCmd = shared::protocol::Parser::parseClientCommand(rawCommand);

    if (const auto* params = std::get_if<ParsedCommandType>(&parsedCmd)) {
        if constexpr (std::is_constructible_v<GuiCommandType, ParsedCommandType>) {
            return std::make_unique<GuiCommandType>(*params);
        } else if constexpr (std::is_default_constructible_v<GuiCommandType>) {
            return std::make_unique<GuiCommandType>();
        }
    }
    return std::make_unique<guiCommand::Sbp>();
}

}  // namespace zappy::server::command
