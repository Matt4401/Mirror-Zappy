/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Food
*/

#pragma once
#include "AObject.hpp"

namespace zappy::gui::graphics::scene {
    class Food : public AObject {
    public:
        Food();
        ~Food();
        Food(const Food& other) = delete;
        Food& operator=(const Food& other) = delete;
        Food(Food&& other) noexcept = default;
        Food& operator=(Food&& other) noexcept = default;

    protected:
    private:
        double _density{0.5};
    };
} // namespace zappy::gui::graphics::scene
