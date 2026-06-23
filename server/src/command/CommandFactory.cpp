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
#include "Set.hpp"
#include "Take.hpp"
#include "command/Fork.hpp"
#include "command/Forward.hpp"
#include "command/ICommand.hpp"
#include "command/Left.hpp"
#include "command/Right.hpp"
#include "guiCommand/Bct.hpp"
#include "guiCommand/IGuiCommand.hpp"
#include "guiCommand/Mct.hpp"
#include "guiCommand/Msz.hpp"
#include "guiCommand/Sgt.hpp"
#include "guiCommand/Sst.hpp"
#include "guiCommand/Tna.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Parser.hpp"

namespace zappy::server::command {

CommandFactory::CommandFactory() {
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

    _guiCreators.emplace("msz", [](std::string_view) { return std::make_unique<guiCommand::Msz>(); });
    _guiCreators.emplace("sgt", [](std::string_view) { return std::make_unique<guiCommand::Sgt>(); });
    _guiCreators.emplace("bct", [](std::string_view rawCommand) -> std::unique_ptr<guiCommand::IGuiCommand> {
        auto parsedCmd = shared::protocol::Parser::parseClientCommand(rawCommand);

        if (const auto* bctParams = std::get_if<shared::protocol::client::Bct>(&parsedCmd)) {
            return std::make_unique<guiCommand::Bct>(bctParams->x, bctParams->y);
        }
        return nullptr;
    });
    _guiCreators.emplace("mct", [](std::string_view) { return std::make_unique<guiCommand::Mct>(); });
    _guiCreators.emplace("tna", [](std::string_view) { return std::make_unique<guiCommand::Tna>(); });
    _guiCreators.emplace("sst", [](std::string_view rawCommand) -> std::unique_ptr<guiCommand::IGuiCommand> {
        auto parsedCmd = shared::protocol::Parser::parseClientCommand(rawCommand);

        if (const auto* sstParams = std::get_if<shared::protocol::client::Sst>(&parsedCmd)) {
            return std::make_unique<guiCommand::Sst>(sstParams->timeUnit);
        }
        return nullptr;
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

}  // namespace zappy::server::command
