/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** EventDispatcher
*/

#pragma once

#include <any>
#include <cstdint>
#include <functional>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "protocol/Commands.hpp"

namespace zappy::gui::events {

class EventDispatcher {
  public:
    using EventToken = uint32_t;

    EventDispatcher() = default;
    ~EventDispatcher() = default;

    EventDispatcher(const EventDispatcher& other) = delete;
    EventDispatcher& operator=(const EventDispatcher& other) = delete;
    EventDispatcher(EventDispatcher&& other) = delete;
    EventDispatcher& operator=(EventDispatcher&& other) = delete;

    template <typename CommandType>
    [[nodiscard]] EventToken subscribe(std::function<void(const CommandType&)> callback) {
        auto type = std::type_index(typeid(CommandType));
        EventToken token = ++_nextToken;

        _listeners[type].push_back({token, std::make_any<std::function<void(const CommandType&)>>(callback)});
        return token;
    }

    template <typename CommandType>
    void unsubscribe(EventToken token) {
        auto type = std::type_index(typeid(CommandType));
        if (auto it = _listeners.find(type); it != _listeners.end()) {
            std::erase_if(it->second, [token](const auto& pair) { return pair.first == token; });
        }
    }

    void dispatch(const shared::protocol::ServerCommand& command) {
        std::visit(
            [this](const auto& cmd) {
                using RealCmdType = std::decay_t<decltype(cmd)>;
                auto type = std::type_index(typeid(RealCmdType));

                if (auto it = _listeners.find(type); it != _listeners.end()) {
                    for (const auto& [token, anyCallback] : it->second) {
                        auto callback = std::any_cast<std::function<void(const RealCmdType&)>>(anyCallback);
                        callback(cmd);
                    }
                }
            },
            command);
    }

  private:
    std::unordered_map<std::type_index, std::vector<std::pair<EventToken, std::any>>> _listeners;
    EventToken _nextToken = 0;
};

}  // namespace zappy::gui::events
