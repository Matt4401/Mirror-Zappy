/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** IObject
*/

#pragma once
#include <string>

#include "rmodels/Model.hpp"

namespace zappy::gui::game {
class IObject {
  public:
    IObject() = default;
    virtual ~IObject() = default;
    IObject(const IObject& other) = delete;
    IObject& operator=(const IObject& other) = delete;
    IObject(IObject&& other) noexcept = default;
    IObject& operator=(IObject&& other) noexcept = default;

    virtual void draw(const raylib::rmodels::Model& model) const = 0;
    [[nodiscard]] virtual std::string name() const = 0;

  protected:
  private:
};
}  // namespace zappy::gui::game
