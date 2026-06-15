/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Mendiane
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
class Mendiane : public AObject {
  public:
    Mendiane(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Mendiane");
        setDensity(0.1);
    }
    ~Mendiane() override = default;
    Mendiane(const Mendiane& other) = delete;
    Mendiane& operator=(const Mendiane& other) = delete;
    Mendiane(Mendiane&& other) noexcept = default;
    Mendiane& operator=(Mendiane&& other) noexcept = default;

  protected:
  private:
};
}  // namespace zappy::gui::game
