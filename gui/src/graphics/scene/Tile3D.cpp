/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Tile3D
*/

#include "Tile3D.hpp"

#include <cstddef>
#include <memory>
#include <utility>

#include "Color.hpp"
#include "components/IObject.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"

namespace zappy::gui::graphics::scene {
Tile3D::Tile3D(raylib::rmath::Vector3 position) : _position{position} {}

void Tile3D::draw(const raylib::rmodels::Model& model) const {
    model.drawModel(_position, 1.0F, raylib::Color::White());
}

bool Tile3D::hasItems() const { return !_itemObjects.empty(); }

void Tile3D::addItem(std::unique_ptr<IObject> object, std::size_t quantity) {
    _itemObjects.emplace_back(Item(std::move(object), static_cast<int>(quantity)));
}

void Tile3D::removeItem(std::size_t index) {
    if (index < 0 || index >= _itemObjects.size()) {
        return;
    }
    _itemObjects.erase(_itemObjects.begin() + static_cast<int>(index));
}

}  // namespace zappy::gui::graphics::scene
