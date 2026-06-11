/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Tile3D
*/

#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "components/IObject.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
class Tile3D {
  public:
    explicit Tile3D(raylib::rmath::Vector3 position = {0.0F, 0.0F, 0.0F});
    ~Tile3D() = default;
    Tile3D(const Tile3D& other) = delete;
    Tile3D& operator=(const Tile3D& other) = delete;
    Tile3D(Tile3D&& other) noexcept = default;
    Tile3D& operator=(Tile3D&& other) noexcept = default;

    void draw(const raylib::rmodels::Model& model) const;

    struct Item {
        std::unique_ptr<IObject> object;
        int quantity{0};
    };

    [[nodiscard]] raylib::rmath::Vector3 position() const { return _position; }
    [[nodiscard]] const std::vector<Item>& items() const { return _itemObjects; }
    [[nodiscard]] bool hasItems() const;

    void addItem(std::unique_ptr<IObject> object, std::size_t quantity = 1);
    void removeItem(std::size_t index);

  protected:
  private:
    raylib::rmath::Vector3 _position;
    std::vector<Item> _itemObjects;
};
}  // namespace zappy::gui::graphics::scene
