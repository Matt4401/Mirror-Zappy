/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** EventContext
*/

#pragma once
#include "rcore/Camera.hpp"
#include <memory>

namespace zappy::gui {
    class EventContext {
    public:
        EventContext(std::shared_ptr<raylib::rcore::Camera> &camera) : _camera(camera) {};
        ~EventContext() = default;
        EventContext(const EventContext& other) = delete;
        EventContext& operator=(const EventContext& other) = delete;
        EventContext(EventContext&& other) = delete;
        EventContext& operator=(EventContext&& other) = delete;

        [[nodiscard]] std::shared_ptr<raylib::rcore::Camera> camera() const { return _camera; }

    protected:
    private:
        std::shared_ptr<raylib::rcore::Camera> _camera;
    };
}  // namespace zappy::gui