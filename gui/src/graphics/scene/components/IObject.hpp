/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** IObject
*/

#pragma once
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
    class IObject {
    public:
        virtual ~IObject();
        IObject(const IObject& other) = delete;
        IObject& operator=(const IObject& other) = delete;
        IObject(IObject&& other) noexcept = default;
        IObject& operator=(IObject&& other) noexcept = default;

        virtual void draw(const raylib::rmodels::Model &model) const = 0;

    protected:
    private:
    };
}