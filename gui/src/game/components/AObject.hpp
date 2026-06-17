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
        model.drawModel(position(), scale(), raylib::Color::White());
    }

    [[nodiscard]] raylib::rmath::Vector3 position() const { return _position; }
    void setPosition(const raylib::rmath::Vector3& position) { _position = position; }
    void setPositionX(float x) { _position.setX(x); }
    void setPositionY(float y) { _position.setY(y); }
    void setPositionZ(float z) { _position.setZ(z); }
    [[nodiscard]] float scale() const { return _scale; }
    void setScale(float scale) { _scale = scale; }
    [[nodiscard]] std::string name() const override { return _name; }
    void setName(const std::string& name) { _name = name; }

  protected:
  private:
    raylib::rmath::Vector3 _position;
    float _scale{0.0};
    std::string _name{"Object"};
};
}  // namespace zappy::gui::game
