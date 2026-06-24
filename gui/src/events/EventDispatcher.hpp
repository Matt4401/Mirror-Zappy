/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** EventDispatcher
*/

#pragma once

#include <cstdint>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <utility>
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

    /**
     * @brief Subscribe to a specific command type with a callback function.
     */
    template <typename CommandType>
    [[nodiscard]] EventToken subscribe(std::move_only_function<void(const CommandType&)> callback) {
        const auto type = std::type_index(typeid(CommandType));
        const EventToken token = _nextToken++;

        std::move_only_function<void(const void*)> erasedCallback =
            [callback = std::move(callback)](const void* event) mutable {
                callback(*static_cast<const CommandType*>(event));
            };

        _listeners[type].push_back(Listener(token, std::move(erasedCallback)));
        return token;
    }

    /**
     * @brief Unsubscribe from a specific command type.
     */
    template <typename CommandType>
    void unsubscribe(EventToken token) {
        const auto type = std::type_index(typeid(CommandType));
        if (auto it = _listeners.find(type); it != _listeners.end()) {
            std::erase_if(it->second, [token](const Listener& listener) { return listener.token == token; });
        }
    }

    template <typename EventType>
    void dispatch(const EventType& event) {
        const auto type = std::type_index(typeid(EventType));

        if (auto it = _listeners.find(type); it != _listeners.end()) {
            for (auto& listener : it->second) {
                listener.callback(&event);
            }
        }
    }

    /**
     * @brief Dispatch a command to all subscribed listeners.
     */
    void dispatch(const shared::protocol::ServerCommand& command);

  private:
    struct Listener {
        EventToken token;
        std::move_only_function<void(const void*)> callback;
    };

    std::unordered_map<std::type_index, std::vector<Listener>> _listeners;
    EventToken _nextToken = 1;
};

}  // namespace zappy::gui::events
