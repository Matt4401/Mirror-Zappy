/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Linemate
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::graphics::scene {
class Linemate : public AObject {
  public:
    Linemate(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Linemate");
        setDensity(0.3);
    }
    ~Linemate() override = default;
    Linemate(const Linemate& other) = delete;
    Linemate& operator=(const Linemate& other) = delete;
    Linemate(Linemate&& other) noexcept = default;
    Linemate& operator=(Linemate&& other) noexcept = default;

  protected:
  private:
};
}  // namespace zappy::gui::graphics::scene
