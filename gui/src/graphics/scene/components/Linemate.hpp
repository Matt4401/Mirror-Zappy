/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Linemate
*/

#pragma once
#include "AObject.hpp"

namespace zappy::gui::graphics::scene {
    class Linemate : public AObject {
    public:
        Linemate();
        ~Linemate();
        Linemate(const Linemate& other) = delete;
        Linemate& operator=(const Linemate& other) = delete;
        Linemate(Linemate&& other) noexcept = default;
        Linemate& operator=(Linemate&& other) noexcept = default;

    protected:
    private:
        double _density{0.3};
    };
} // namespace zappy::gui::graphics::scene