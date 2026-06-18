/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** AssetManager
*/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "rtext/Font.hpp"

namespace zappy::gui::graphics {

class AssetManager {
  public:
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;

    static AssetManager& getInstance();

    bool loadFont(const std::string& id, const std::string& path);
    std::shared_ptr<raylib::rtext::Font> getFont(const std::string& id);
    void clear();

  private:
    AssetManager() = default;
    ~AssetManager() = default;
    std::unordered_map<std::string, std::shared_ptr<raylib::rtext::Font>> _fonts;
};

}  // namespace zappy::gui::graphics
