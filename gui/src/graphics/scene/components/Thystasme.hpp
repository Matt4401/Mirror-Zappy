/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Thystasme
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
class Thystasme : public AObject {
  public:
    Thystasme(raylib::rmath::Vector3 position) : AObject(position) { setName("Thystasme"); }
    ~Thystasme() override = default;
    Thystasme(const Thystasme& other) = delete;
    Thystasme& operator=(const Thystasme& other) = delete;
    Thystasme(Thystasme&& other) noexcept = default;
    Thystasme& operator=(Thystasme&& other) noexcept = default;

  protected:
  private:
    double _density{0.05};
};
}  // namespace zappy::gui::graphics::scene
