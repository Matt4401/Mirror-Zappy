/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Egg
*/

#pragma once
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
    class Egg {
        public:
            Egg() = default;
            ~Egg() = default;
            Egg(const Egg& other) = delete;
            Egg& operator=(const Egg& other) = delete;
            Egg(Egg&& other) noexcept = default;
            Egg& operator=(Egg&& other) noexcept = default;

            [[nodiscard]] raylib::rmath::Vector3 position() const { return {_position.x(), _position.y(), _position.z()}; }
        protected:
        private:
            raylib::rmath::Vector3 _position {0.0F, 4.0F, 0.0F};
    };
}