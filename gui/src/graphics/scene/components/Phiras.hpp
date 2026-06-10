/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Phiras
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
class Phiras : public AObject {
  public:
    Phiras(raylib::rmath::Vector3 position) : AObject(position) { setName("Phiras"); }
    ~Phiras() override = default;
    Phiras(const Phiras& other) = delete;
    Phiras& operator=(const Phiras& other) = delete;
    Phiras(Phiras&& other) noexcept = default;
    Phiras& operator=(Phiras&& other) noexcept = default;

  protected:
  private:
    double _density{0.08};
};
}  // namespace zappy::gui::graphics::scene
