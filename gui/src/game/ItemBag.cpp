/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** ItemBag
*/

#include "ItemBag.hpp"

#include <cstddef>
#include <memory>
#include <string_view>
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

std::size_t ItemBag::quantity(const std::string_view name) const {
    for (const auto& item : _items) {
        if (item.object->name() == name) {
            return item.quantity;
        }
    }
    return 0;
}

void ItemBag::removeItem(const std::string_view name, const std::size_t quantity) {
    for (auto it = _items.begin(); it != _items.end(); ++it) {
        if (it->object->name() != name) {
            continue;
        }
        if (it->quantity > quantity) {
            it->quantity -= quantity;
        } else {
            _items.erase(it);
        }
        return;
    }
}
}  // namespace zappy::gui::game
