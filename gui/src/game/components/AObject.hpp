/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** AObject
*/

#pragma once
#include <string>

#include "Color.hpp"
#include "IObject.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::game {
class AObject : public IObject {
  public:
    AObject(raylib::rmath::Vector3 position) : _position(position) {}
    ~AObject() override = default;
    AObject(const AObject& other) = delete;
    AObject& operator=(const AObject& other) = delete;
    AObject(AObject&& other) noexcept = default;
    AObject& operator=(AObject&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const override {
        model.drawModel(_position, 1.0F, raylib::Color::White());
    }

    [[nodiscard]] raylib::rmath::Vector3 position() const { return _position; }
    void setPosition(const raylib::rmath::Vector3& position) { _position = position; }
    void setPositionX(float x) { _position.setX(x); }
    void setPositionY(float y) { _position.setY(y); }
    void setPositionZ(float z) { _position.setZ(z); }
    [[nodiscard]] double density() const { return _density; }
    void setDensity(double density) { _density = density; }
    [[nodiscard]] std::string name() const override { return _name; }
    void setName(const std::string& name) { _name = name; }

  protected:
  private:
    raylib::rmath::Vector3 _position;
    double _density{0.0};
    std::string _name{"Object"};
};
}  // namespace zappy::gui::game
