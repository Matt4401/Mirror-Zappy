/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** ItemBag
*/

#pragma once
#include <cstddef>
#include <memory>
#include <vector>

#include "components/IObject.hpp"

namespace zappy::gui::game {
class ItemBag {
  public:
    ItemBag() = default;
    ~ItemBag() = default;
    ItemBag(const ItemBag& other) = delete;
    ItemBag& operator=(const ItemBag& other) = delete;
    ItemBag(ItemBag&& other) noexcept = default;
    ItemBag& operator=(ItemBag&& other) noexcept = default;

    struct Item {
        std::unique_ptr<IObject> object;
        std::size_t quantity{0};
    };

    [[nodiscard]] bool hasItems() const { return !_items.empty(); }
    [[nodiscard]] const std::vector<Item>& items() const { return _items; }

    void addItem(std::unique_ptr<zappy::gui::game::IObject> object, std::size_t quantity = 1);
    void removeItem(std::size_t index);

  protected:
  private:
    std::vector<Item> _items;
};
}  // namespace zappy::gui::game
