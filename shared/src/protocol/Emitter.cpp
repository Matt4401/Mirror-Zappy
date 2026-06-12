/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** protocol emitter implementation
*/

#include "protocol/Emitter.hpp"

#include <string>
#include <variant>

#include "protocol/Commands.hpp"

namespace zappy::shared::protocol {

namespace {
template <class... Ts>
struct overloaded : Ts... {  // NOLINT(cppcoreguidelines-multiple-inheritance)
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
}  // namespace

std::string Emitter::build(const ServerCommand& cmd) {
    return std::visit(
        overloaded{[](const server::Msz& c) {
                       return "msz " + std::to_string(c.width) + " " + std::to_string(c.height) + "\n";
                   },
                   [](const server::Bct& c) {
                       return "bct " + std::to_string(c.x) + " " + std::to_string(c.y) + " " + std::to_string(c.food) +
                              " " + std::to_string(c.linemate) + " " + std::to_string(c.deraumere) + " " +
                              std::to_string(c.sibur) + " " + std::to_string(c.mendiane) + " " +
                              std::to_string(c.phiras) + " " + std::to_string(c.thystame) + "\n";
                   },
                   [](const server::Tna& c) { return "tna " + c.teamName + "\n"; },
                   [](const server::Pnw& c) {
                       return "pnw " + std::to_string(c.playerId) + " " + std::to_string(c.x) + " " +
                              std::to_string(c.y) + " " + std::to_string(c.orientation) + " " +
                              std::to_string(c.level) + " " + c.teamName + "\n";
                   },
                   [](const server::Ppo& c) {
                       return "ppo " + std::to_string(c.playerId) + " " + std::to_string(c.x) + " " +
                              std::to_string(c.y) + " " + std::to_string(c.orientation) + "\n";
                   },
                   [](const server::Plv& c) {
                       return "plv " + std::to_string(c.playerId) + " " + std::to_string(c.level) + "\n";
                   },
                   [](const server::Pin& c) {
                       return "pin " + std::to_string(c.playerId) + " " + std::to_string(c.x) + " " +
                              std::to_string(c.y) + " " + std::to_string(c.food) + " " + std::to_string(c.linemate) +
                              " " + std::to_string(c.deraumere) + " " + std::to_string(c.sibur) + " " +
                              std::to_string(c.mendiane) + " " + std::to_string(c.phiras) + " " +
                              std::to_string(c.thystame) + "\n";
                   },
                   [](const server::Pex& c) { return "pex " + std::to_string(c.playerId) + "\n"; },
                   [](const server::Pbc& c) { return "pbc " + std::to_string(c.playerId) + " " + c.message + "\n"; },
                   [](const server::Pic& c) {
                       std::string s =
                           "pic " + std::to_string(c.x) + " " + std::to_string(c.y) + " " + std::to_string(c.level);
                       for (int const p : c.playerIds) {
                           s += " " + std::to_string(p);
                       }
                       return s + "\n";
                   },
                   [](const server::Pie& c) {
                       return "pie " + std::to_string(c.x) + " " + std::to_string(c.y) + " " +
                              std::to_string(c.incantationResult) + "\n";
                   },
                   [](const server::Pfk& c) { return "pfk " + std::to_string(c.playerId) + "\n"; },
                   [](const server::Pdr& c) {
                       return "pdr " + std::to_string(c.playerId) + " " + std::to_string(c.resourceId) + "\n";
                   },
                   [](const server::Pgt& c) {
                       return "pgt " + std::to_string(c.playerId) + " " + std::to_string(c.resourceId) + "\n";
                   },
                   [](const server::Pdi& c) { return "pdi " + std::to_string(c.playerId) + "\n"; },
                   [](const server::Enw& c) {
                       return "enw " + std::to_string(c.eggId) + " " + std::to_string(c.playerId) + " " +
                              std::to_string(c.x) + " " + std::to_string(c.y) + "\n";
                   },
                   [](const server::Ebo& c) { return "ebo " + std::to_string(c.eggId) + "\n"; },
                   [](const server::Edi& c) { return "edi " + std::to_string(c.eggId) + "\n"; },
                   [](const server::Sgt& c) { return "sgt " + std::to_string(c.timeUnit) + "\n"; },
                   [](const server::Sst& c) { return "sst " + std::to_string(c.timeUnit) + "\n"; },
                   [](const server::Seg& c) { return "seg " + c.teamName + "\n"; },
                   [](const server::Smg& c) { return "smg " + c.message + "\n"; },
                   [](const server::Suc&) { return std::string("suc\n"); },
                   [](const server::Sbp&) { return std::string("sbp\n"); },
                   [](const UnknownCommand&) { return std::string("suc\n"); }},
        cmd);
}

std::string Emitter::build(const ClientCommand& cmd) {
    return std::visit(
        overloaded{[](const client::Msz&) { return std::string("msz\n"); },
                   [](const client::Bct& c) { return "bct " + std::to_string(c.x) + " " + std::to_string(c.y) + "\n"; },
                   [](const client::Mct&) { return std::string("mct\n"); },
                   [](const client::Tna&) { return std::string("tna\n"); },
                   [](const client::Ppo& c) { return "ppo " + std::to_string(c.playerId) + "\n"; },
                   [](const client::Plv& c) { return "plv " + std::to_string(c.playerId) + "\n"; },
                   [](const client::Pin& c) { return "pin " + std::to_string(c.playerId) + "\n"; },
                   [](const client::Sgt&) { return std::string("sgt\n"); },
                   [](const client::Sst& c) { return "sst " + std::to_string(c.timeUnit) + "\n"; },
                   [](const UnknownCommand&) { return std::string("suc\n"); }},
        cmd);
}

}  // namespace zappy::shared::protocol
