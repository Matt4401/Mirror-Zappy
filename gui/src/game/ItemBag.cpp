/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** ItemBag
*/

#include "ItemBag.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "components/IObject.hpp"

namespace zappy::gui::game {
void ItemBag::addItem(std::unique_ptr<zappy::gui::game::IObject> object, std::size_t quantity) {
    if (!object) {
        return;
    }
    for (auto& item : _items) {
        if (item.object->name() == object->name()) {
            item.quantity += quantity;
            return;
        }
    }
    _items.emplace_back(Item(std::move(object), quantity));
}

void ItemBag::removeItem(std::size_t index) {
    if (index >= _items.size()) {
        return;
    }
    _items.erase(_items.begin() + static_cast<std::vector<Item>::difference_type>(index));
}
}  // namespace zappy::gui::game
