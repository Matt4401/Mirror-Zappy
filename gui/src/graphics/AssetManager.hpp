/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** AssetManager
*/

#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "rtext/Font.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::graphics {

class AssetManager {
  public:
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;

    static AssetManager& getInstance();

    bool loadFont(std::string_view id, std::string_view path);
    bool loadTexture(std::string_view id, std::string_view path);

    std::shared_ptr<raylib::rtext::Font> getFont(std::string_view id);
    std::shared_ptr<raylib::rtextures::Texture2D> getTexture(std::string_view id);
    void clear();

  private:
    AssetManager() = default;
    ~AssetManager() = default;
    std::unordered_map<std::string, std::shared_ptr<raylib::rtext::Font>> _fonts;
    std::unordered_map<std::string, std::shared_ptr<raylib::rtextures::Texture2D>> _textures;
};

}  // namespace zappy::gui::graphics
