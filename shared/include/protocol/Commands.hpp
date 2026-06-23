/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** protocol commands structures
*/

#pragma once

#include <string>
#include <variant>
#include <vector>

namespace zappy::shared::protocol {

// --- Error / Unknown ---
struct UnknownCommand {};

// --- Server to Client (GUI) Commands ---
namespace server {
struct Msz {
    int width = 0;
    int height = 0;
};
struct Bct {
    int x = 0;
    int y = 0;
    int food = 0;
    int linemate = 0;
    int deraumere = 0;
    int sibur = 0;
    int mendiane = 0;
    int phiras = 0;
    int thystame = 0;
};
struct Tna {
    std::string teamName;
};
struct Pnw {
    int playerId = 0;
    int x = 0;
    int y = 0;
    int orientation = 0;
    int level = 0;
    std::string teamName;
};
struct Ppo {
    int playerId = 0;
    int x = 0;
    int y = 0;
    int orientation = 0;
};
struct Plv {
    int playerId = 0;
    int level = 0;
};
struct Pin {
    int playerId = 0;
    int x = 0;
    int y = 0;
    int food = 0;
    int linemate = 0;
    int deraumere = 0;
    int sibur = 0;
    int mendiane = 0;
    int phiras = 0;
    int thystame = 0;
};
struct Pex {
    int playerId = 0;
};
struct Pbc {
    int playerId = 0;
    std::string message;
};
struct Pic {
    int x = 0;
    int y = 0;
    int level = 0;
    std::vector<int> playerIds;
};
struct Pie {
    int x = 0;
    int y = 0;
    bool incantationResult = 0;
};
struct Pfk {
    int playerId = 0;
};
struct Pdr {
    int playerId = 0;
    int resourceId = 0;
};
struct Pgt {
    int playerId = 0;
    int resourceId = 0;
};
struct Pdi {
    int playerId = 0;
};
struct Enw {
    int eggId = 0;
    int playerId = 0;
    int x = 0;
    int y = 0;
};
struct Ebo {
    int eggId = 0;
};
struct Edi {
    int eggId = 0;
};
struct Sgt {
    int timeUnit = 0;
};
struct Sst {
    int timeUnit = 0;
};
struct Seg {
    std::string teamName;
};
struct Smg {
    std::string message;
};
struct Suc {};
struct Sbp {};
}  // namespace server

// --- Client (GUI) to Server Commands ---
namespace client {
struct Msz {};
struct Bct {
    int x = 0;
    int y = 0;
};
struct Mct {};
struct Tna {};
struct Ppo {
    int playerId = 0;
};
struct Plv {
    int playerId = 0;
};
struct Pin {
    int playerId = 0;
};
struct Sgt {};
struct Sst {
    int timeUnit = 0;
};
}  // namespace client

// The variant grouping all Server to GUI commands
using ServerCommand =
    std::variant<UnknownCommand, server::Msz, server::Bct, server::Tna, server::Pnw, server::Ppo, server::Plv,
                 server::Pin, server::Pex, server::Pbc, server::Pic, server::Pie, server::Pfk, server::Pdr, server::Pgt,
                 server::Pdi, server::Enw, server::Ebo, server::Edi, server::Sgt, server::Sst, server::Seg, server::Smg,
                 server::Suc, server::Sbp>;

// The variant grouping all GUI to Server commands
using ClientCommand = std::variant<UnknownCommand, client::Msz, client::Bct, client::Mct, client::Tna, client::Ppo,
                                   client::Plv, client::Pin, client::Sgt, client::Sst>;

}  // namespace zappy::shared::protocol
