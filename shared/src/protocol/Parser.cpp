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

#include "protocol/Commands.hpp"

namespace zappy::shared::protocol {

namespace {

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
    if (iss >> cmd.playerId >> cmd.x >> cmd.y >> cmd.orientation >> cmd.level >> cmd.teamName) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPpo(std::istringstream& iss) {
    server::Ppo cmd{};
    if (iss >> cmd.playerId >> cmd.x >> cmd.y >> cmd.orientation) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPlv(std::istringstream& iss) {
    server::Plv cmd{};
    if (iss >> cmd.playerId >> cmd.level) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPin(std::istringstream& iss) {
    server::Pin cmd{};
    if (iss >> cmd.playerId >> cmd.x >> cmd.y >> cmd.food >> cmd.linemate >> cmd.deraumere >> cmd.sibur >>
        cmd.mendiane >> cmd.phiras >> cmd.thystame) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPex(std::istringstream& iss) {
    server::Pex cmd{};
    if (iss >> cmd.playerId) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPbc(std::istringstream& iss) {
    server::Pbc cmd{};
    if (iss >> cmd.playerId) {
        std::string msg;
        std::getline(iss, msg);
        if (!msg.empty() && msg[0] == ' ') {
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
        while (iss >> p) {
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
    if (iss >> cmd.playerId) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPdr(std::istringstream& iss) {
    server::Pdr cmd{};
    if (iss >> cmd.playerId >> cmd.resourceId) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPgt(std::istringstream& iss) {
    server::Pgt cmd{};
    if (iss >> cmd.playerId >> cmd.resourceId) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerPdi(std::istringstream& iss) {
    server::Pdi cmd{};
    if (iss >> cmd.playerId) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerEnw(std::istringstream& iss) {
    server::Enw cmd{};
    if (iss >> cmd.eggId >> cmd.playerId >> cmd.x >> cmd.y) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerEbo(std::istringstream& iss) {
    server::Ebo cmd{};
    if (iss >> cmd.eggId) {
        return cmd;
    }
    return UnknownCommand{};
}

ServerCommand parseServerEdi(std::istringstream& iss) {
    server::Edi cmd{};
    if (iss >> cmd.eggId) {
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
    if (!msg.empty() && msg[0] == ' ') {
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
    if (iss >> cmd.x >> cmd.y) {
        return cmd;
    }
    return UnknownCommand{};
}

ClientCommand parseClientMct(std::istringstream& /*unused*/) { return client::Mct{}; }
ClientCommand parseClientTna(std::istringstream& /*unused*/) { return client::Tna{}; }

ClientCommand parseClientPpo(std::istringstream& iss) {
    client::Ppo cmd{};
    if (iss >> cmd.playerId) {
        return cmd;
    }
    return UnknownCommand{};
}

ClientCommand parseClientPlv(std::istringstream& iss) {
    client::Plv cmd{};
    if (iss >> cmd.playerId) {
        return cmd;
    }
    return UnknownCommand{};
}

ClientCommand parseClientPin(std::istringstream& iss) {
    client::Pin cmd{};
    if (iss >> cmd.playerId) {
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

Parser::Parser() {
    initServerDispatch();
    initClientDispatch();
}

ServerCommand Parser::parseServerCommand(std::string_view input) const {
    std::string const inputStr(input);
    std::istringstream iss(inputStr);
    std::string command;

    if (!(iss >> command)) {
        return UnknownCommand{};
    }

    auto it = _serverDispatch.find(command);
    if (it != _serverDispatch.end()) {
        return it->second(iss);
    }
    return UnknownCommand{};
}

ClientCommand Parser::parseClientCommand(std::string_view input) const {
    std::string const inputStr(input);
    std::istringstream iss(inputStr);
    std::string command;

    if (!(iss >> command)) {
        return UnknownCommand{};
    }

    auto it = _clientDispatch.find(command);
    if (it != _clientDispatch.end()) {
        return it->second(iss);
    }
    return UnknownCommand{};
}

void Parser::initServerDispatch() {
    _serverDispatch["msz"] = parseServerMsz;
    _serverDispatch["bct"] = parseServerBct;
    _serverDispatch["tna"] = parseServerTna;
    _serverDispatch["pnw"] = parseServerPnw;
    _serverDispatch["ppo"] = parseServerPpo;
    _serverDispatch["plv"] = parseServerPlv;
    _serverDispatch["pin"] = parseServerPin;
    _serverDispatch["pex"] = parseServerPex;
    _serverDispatch["pbc"] = parseServerPbc;
    _serverDispatch["pic"] = parseServerPic;
    _serverDispatch["pie"] = parseServerPie;
    _serverDispatch["pfk"] = parseServerPfk;
    _serverDispatch["pdr"] = parseServerPdr;
    _serverDispatch["pgt"] = parseServerPgt;
    _serverDispatch["pdi"] = parseServerPdi;
    _serverDispatch["enw"] = parseServerEnw;
    _serverDispatch["ebo"] = parseServerEbo;
    _serverDispatch["edi"] = parseServerEdi;
    _serverDispatch["sgt"] = parseServerSgt;
    _serverDispatch["sst"] = parseServerSst;
    _serverDispatch["seg"] = parseServerSeg;
    _serverDispatch["smg"] = parseServerSmg;
    _serverDispatch["suc"] = parseServerSuc;
    _serverDispatch["sbp"] = parseServerSbp;
}

void Parser::initClientDispatch() {
    _clientDispatch["msz"] = parseClientMsz;
    _clientDispatch["bct"] = parseClientBct;
    _clientDispatch["mct"] = parseClientMct;
    _clientDispatch["tna"] = parseClientTna;
    _clientDispatch["ppo"] = parseClientPpo;
    _clientDispatch["plv"] = parseClientPlv;
    _clientDispatch["pin"] = parseClientPin;
    _clientDispatch["sgt"] = parseClientSgt;
    _clientDispatch["sst"] = parseClientSst;
}

}  // namespace zappy::shared::protocol
