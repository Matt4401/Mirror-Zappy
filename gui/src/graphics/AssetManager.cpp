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

#include "rtext/Font.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::graphics {

AssetManager& AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

bool AssetManager::loadFont(const std::string& id, const std::string& path) {
    if (_fonts.contains(id)) {
        return true;
    }
    auto font = std::make_shared<raylib::rtext::Font>(path);
    if (!font->valid()) {
        std::cerr << "Failed to load font: " << path << std::endl;
        return false;
    }
    _fonts.insert_or_assign(id, font);
    return true;
}

bool AssetManager::loadTexture(const std::string& id, const std::string& path) {
    if (_textures.contains(id)) {
        return true;
    }
    auto texture = std::make_shared<raylib::rtextures::Texture2D>(path);
    if (!texture->valid()) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return false;
    }
    _textures.insert_or_assign(id, texture);
    return true;
}

std::shared_ptr<raylib::rtext::Font> AssetManager::getFont(const std::string& id) {
    if (_fonts.contains(id)) {
        return _fonts.at(id);
    }
    return nullptr;
}

std::shared_ptr<raylib::rtextures::Texture2D> AssetManager::getTexture(const std::string& id) {
    auto it = _textures.find(id);
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
