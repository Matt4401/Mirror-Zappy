/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** protocol parser implementation
*/

#include "protocol/Parser.hpp"

#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

#include "protocol/Commands.hpp"

namespace zappy::shared::protocol {

namespace {

using ServerParseFunc = ServerCommand (*)(std::istringstream&);
using ClientParseFunc = ClientCommand (*)(std::istringstream&);

bool extractHashId(std::istringstream& iss, int& id) {
    char hash = 0;
    if (iss >> hash && hash == '#') {
        if (iss >> id) {
            return true;
        }
    }
    return false;
}

ServerCommand parseServerMsz(std::istringstream& iss) {
    server::Msz cmd{};
    if (iss >> cmd.width >> cmd.height) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerBct(std::istringstream& iss) {
    server::Bct cmd{};
    if (iss >> cmd.x >> cmd.y >> cmd.food >> cmd.linemate >> cmd.deraumere >> cmd.sibur >> cmd.mendiane >> cmd.phiras >>
        cmd.thystame) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerTna(std::istringstream& iss) {
    server::Tna cmd{};
    if (iss >> cmd.teamName) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPnw(std::istringstream& iss) {
    server::Pnw cmd{};
    if (extractHashId(iss, cmd.playerId) && iss >> cmd.x >> cmd.y >> cmd.orientation >> cmd.level >> cmd.teamName) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPpo(std::istringstream& iss) {
    server::Ppo cmd{};
    if (extractHashId(iss, cmd.playerId) && iss >> cmd.x >> cmd.y >> cmd.orientation) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPlv(std::istringstream& iss) {
    server::Plv cmd{};
    if (extractHashId(iss, cmd.playerId) && iss >> cmd.level) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPin(std::istringstream& iss) {
    server::Pin cmd{};
    if (extractHashId(iss, cmd.playerId) && iss >> cmd.x >> cmd.y >> cmd.food >> cmd.linemate >> cmd.deraumere >>
                                                cmd.sibur >> cmd.mendiane >> cmd.phiras >> cmd.thystame) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPex(std::istringstream& iss) {
    server::Pex cmd{};
    if (extractHashId(iss, cmd.playerId)) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPbc(std::istringstream& iss) {
    server::Pbc cmd{};
    if (extractHashId(iss, cmd.playerId)) {
        std::string msg;
        std::getline(iss, msg);
        if (!msg.empty() && msg.front() == ' ') {
            msg.erase(0, 1);
        }
        cmd.message = msg;
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPic(std::istringstream& iss) {
    server::Pic cmd{};
    if (iss >> cmd.x >> cmd.y >> cmd.level) {
        int p = 0;
        while (extractHashId(iss, p)) {
            cmd.playerIds.push_back(p);
        }
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPie(std::istringstream& iss) {
    server::Pie cmd{};
    if (iss >> cmd.x >> cmd.y >> cmd.incantationResult) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPfk(std::istringstream& iss) {
    server::Pfk cmd{};
    if (extractHashId(iss, cmd.playerId)) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPdr(std::istringstream& iss) {
    server::Pdr cmd{};
    if (extractHashId(iss, cmd.playerId) && iss >> cmd.resourceId) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPgt(std::istringstream& iss) {
    server::Pgt cmd{};
    if (extractHashId(iss, cmd.playerId) && iss >> cmd.resourceId) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPdi(std::istringstream& iss) {
    server::Pdi cmd{};
    if (extractHashId(iss, cmd.playerId)) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerEnw(std::istringstream& iss) {
    server::Enw cmd{};
    if (extractHashId(iss, cmd.eggId) && extractHashId(iss, cmd.playerId) && iss >> cmd.x >> cmd.y) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerEbo(std::istringstream& iss) {
    server::Ebo cmd{};
    if (extractHashId(iss, cmd.eggId)) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerEdi(std::istringstream& iss) {
    server::Edi cmd{};
    if (extractHashId(iss, cmd.eggId)) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerSgt(std::istringstream& iss) {
    server::Sgt cmd{};
    if (iss >> cmd.timeUnit) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerSst(std::istringstream& iss) {
    server::Sst cmd{};
    if (iss >> cmd.timeUnit) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerSeg(std::istringstream& iss) {
    server::Seg cmd{};
    if (iss >> cmd.teamName) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerSmg(std::istringstream& iss) {
    server::Smg cmd{};
    std::string msg;
    std::getline(iss, msg);
    if (!msg.empty() && msg.front() == ' ') {
        msg.erase(0, 1);
    }
    cmd.message = msg;
    return cmd;
}

ServerCommand parseServerSuc(std::istringstream& /*unused*/) { return server::Suc{}; }
ServerCommand parseServerSbp(std::istringstream& /*unused*/) { return server::Sbp{}; }

ClientCommand parseClientMsz(std::istringstream& /*unused*/) { return client::Msz{}; }

ClientCommand parseClientBct(std::istringstream& iss) {
    client::Bct cmd{};
    if (iss >> cmd.x >> cmd.y && iss.eof()) {
        return cmd;
    }
    return UnknownCommand{};
}

ClientCommand parseClientMct(std::istringstream& /*unused*/) { return client::Mct{}; }
ClientCommand parseClientTna(std::istringstream& /*unused*/) { return client::Tna{}; }

ClientCommand parseClientPpo(std::istringstream& iss) {
    client::Ppo cmd{};
    if (extractHashId(iss, cmd.playerId)) {
        return cmd;
    }
    return UnknownCommand{};
}

ClientCommand parseClientPlv(std::istringstream& iss) {
    client::Plv cmd{};
    if (extractHashId(iss, cmd.playerId)) {
        return cmd;
    }
    return UnknownCommand{};
}

ClientCommand parseClientPin(std::istringstream& iss) {
    client::Pin cmd{};
    if (extractHashId(iss, cmd.playerId)) {
        return cmd;
    }
    return UnknownCommand{};
}

ClientCommand parseClientSgt(std::istringstream& /*unused*/) { return client::Sgt{}; }

ClientCommand parseClientSst(std::istringstream& iss) {
    client::Sst cmd{};
    if (iss >> cmd.timeUnit) {
        return cmd;
    }
    return UnknownCommand{};
}

}  // namespace

ServerCommand Parser::parseServerCommand(std::string_view input) {
    static const std::unordered_map<std::string, ServerParseFunc> serverDispatch = {
        {"msz", parseServerMsz}, {"bct", parseServerBct}, {"tna", parseServerTna}, {"pnw", parseServerPnw},
        {"ppo", parseServerPpo}, {"plv", parseServerPlv}, {"pin", parseServerPin}, {"pex", parseServerPex},
        {"pbc", parseServerPbc}, {"pic", parseServerPic}, {"pie", parseServerPie}, {"pfk", parseServerPfk},
        {"pdr", parseServerPdr}, {"pgt", parseServerPgt}, {"pdi", parseServerPdi}, {"enw", parseServerEnw},
        {"ebo", parseServerEbo}, {"edi", parseServerEdi}, {"sgt", parseServerSgt}, {"sst", parseServerSst},
        {"seg", parseServerSeg}, {"smg", parseServerSmg}, {"suc", parseServerSuc}, {"sbp", parseServerSbp}};

    std::string const inputStr(input);
    std::istringstream iss(inputStr);
    std::string command;

    if (!(iss >> command)) {
        return UnknownCommand{};
    }

    auto const it = serverDispatch.find(command);
    if (it != serverDispatch.end()) {
        return it->second(iss);
    }
    return UnknownCommand{};
}

ClientCommand Parser::parseClientCommand(std::string_view input) {
    static const std::unordered_map<std::string, ClientParseFunc> clientDispatch = {
        {"msz", parseClientMsz}, {"bct", parseClientBct}, {"mct", parseClientMct},
        {"tna", parseClientTna}, {"ppo", parseClientPpo}, {"plv", parseClientPlv},
        {"pin", parseClientPin}, {"sgt", parseClientSgt}, {"sst", parseClientSst}};

    std::string const inputStr(input);
    std::istringstream iss(inputStr);
    std::string command;

    if (!(iss >> command)) {
        return UnknownCommand{};
    }

    auto const it = clientDispatch.find(command);
    if (it != clientDispatch.end()) {
        return it->second(iss);
    }
    return UnknownCommand{};
}

}  // namespace zappy::shared::protocol
