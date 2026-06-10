/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Deraumere
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "Color.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
class Deraumere : public AObject {
  public:
    Deraumere(raylib::rmath::Vector3 position) : AObject(position) { setName("Deraumere"); }
    ~Deraumere() override = default;
    Deraumere(const Deraumere& other) = delete;
    Deraumere& operator=(const Deraumere& other) = delete;
    Deraumere(Deraumere&& other) noexcept = default;
    Deraumere& operator=(Deraumere&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const override {
        model.drawModel(position(), 0.05F, raylib::Color::White());
    }

  protected:
  private:
    double _density{0.15};
};
}  // namespace zappy::gui::graphics::scene
