/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Sibur
*/

#pragma once
#include <string>

#include "AObject.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui::game {
class Sibur : public AObject {
  public:
    Sibur(raylib::rmath::Vector3 position) : AObject(position) {
        setName("Sibur");
        setDensity(0.1);
    }
    ~Sibur() override = default;
    Sibur(const Sibur& other) = delete;
    Sibur& operator=(const Sibur& other) = delete;
    Sibur(Sibur&& other) noexcept = default;
    Sibur& operator=(Sibur&& other) noexcept = default;

  protected:
  private:
};
}  // namespace zappy::gui::game
