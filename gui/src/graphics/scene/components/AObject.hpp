/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** AObject
*/

#pragma once
#include "IObject.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"
#include "Color.hpp"

namespace zappy::gui::graphics::scene {
    class AObject : public IObject {
    public:
        AObject();
        ~AObject();
        AObject(const AObject& other) = delete;
        AObject& operator=(const AObject& other) = delete;
        AObject(AObject&& other) noexcept = default;
        AObject& operator=(AObject&& other) noexcept = default;

        void draw(const raylib::rmodels::Model &model) const override {
            model.drawModel(_position, 1.0F, raylib::Color::White());
        }

        [[nodiscard]] raylib::rmath::Vector3 position() const { return _position; }
        void setPosition(const raylib::rmath::Vector3& position) { _position = position; }
        [[nodiscard]] double density() const { return _density; }
        void setDensity(double density) { _density = density; }

    protected:
    private:
        raylib::rmath::Vector3 _position;
        double _density;
    };
}