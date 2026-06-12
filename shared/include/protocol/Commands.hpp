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
    int width;
    int height;
};
struct Bct {
    int x;
    int y;
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
};
struct Tna {
    std::string teamName;
};
struct Pnw {
    int playerId;
    int x;
    int y;
    int orientation;
    int level;
    std::string teamName;
};
struct Ppo {
    int playerId;
    int x;
    int y;
    int orientation;
};
struct Plv {
    int playerId;
    int level;
};
struct Pin {
    int playerId;
    int x;
    int y;
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
};
struct Pex {
    int playerId;
};
struct Pbc {
    int playerId;
    std::string message;
};
struct Pic {
    int x;
    int y;
    int level;
    std::vector<int> playerIds;
};
struct Pie {
    int x;
    int y;
    int incantationResult;
};
struct Pfk {
    int playerId;
};
struct Pdr {
    int playerId;
    int resourceId;
};
struct Pgt {
    int playerId;
    int resourceId;
};
struct Pdi {
    int playerId;
};
struct Enw {
    int eggId;
    int playerId;
    int x;
    int y;
};
struct Ebo {
    int eggId;
};
struct Edi {
    int eggId;
};
struct Sgt {
    int timeUnit;
};
struct Sst {
    int timeUnit;
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
    int x;
    int y;
};
struct Mct {};
struct Tna {};
struct Ppo {
    int playerId;
};
struct Plv {
    int playerId;
};
struct Pin {
    int playerId;
};
struct Sgt {};
struct Sst {
    int timeUnit;
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
