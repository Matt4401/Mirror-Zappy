/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** ItemBag
*/

#include "ItemBag.hpp"
#include "components/IObject.hpp"
#include <vector>
#include <memory>
#include <cstddef>
#include <utility>

namespace zappy::gui::game {
    void ItemBag::addItem(std::unique_ptr<zappy::gui::game::IObject> object, std::size_t quantity) {
        for (auto& item : _items) {
            if (item.object->name() == object->name()) {
                item.quantity += static_cast<int>(quantity);
                return;
            }
        }
        _items.emplace_back(Item(std::move(object), static_cast<int>(quantity)));
    }

    void ItemBag::removeItem(std::size_t index) {
        if (index >= _items.size()) {
            return;
        }
        _items.erase(_items.begin() + static_cast<int>(index));
    }
}