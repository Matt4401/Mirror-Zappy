/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** AssetManager
*/

#include "AssetManager.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include "rtext/Font.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::graphics {

AssetManager& AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

bool AssetManager::loadFont(const std::string_view id, const std::string_view path) {
    const std::string assetId{id};
    if (_fonts.contains(assetId)) {
        return true;
    }
    auto font = std::make_shared<raylib::rtext::Font>(std::string{path});
    if (!font->valid()) {
        std::cerr << "Failed to load font: " << path << std::endl;
        return false;
    }
    _fonts.insert_or_assign(assetId, font);
    return true;
}

bool AssetManager::loadTexture(const std::string_view id, const std::string_view path) {
    const std::string assetId{id};
    if (_textures.contains(assetId)) {
        return true;
    }
    auto texture = std::make_shared<raylib::rtextures::Texture2D>(std::string{path});
    if (!texture->valid()) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return false;
    }
    _textures.insert_or_assign(assetId, texture);
    return true;
}

std::shared_ptr<raylib::rtext::Font> AssetManager::getFont(const std::string_view id) {
    const std::string assetId{id};
    if (_fonts.contains(assetId)) {
        return _fonts.at(assetId);
    }
    return nullptr;
}

std::shared_ptr<raylib::rtextures::Texture2D> AssetManager::getTexture(const std::string_view id) {
    auto it = _textures.find(std::string{id});
    if (it != _textures.end()) {
        return it->second;
    }
    return nullptr;
}

void AssetManager::clear() {
    _fonts.clear();
    _textures.clear();
}

}  // namespace zappy::gui::graphics
