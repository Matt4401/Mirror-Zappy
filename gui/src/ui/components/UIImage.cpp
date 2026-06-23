/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIImage
*/

#include "UIImage.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "Color.hpp"
#include "graphics/AssetManager.hpp"
#include "rcore/Event.hpp"

namespace zappy::gui::ui::components {

UIImage::UIImage(const std::string& path) : _path(path) {
    try {
        if (!path.empty()) {
            graphics::AssetManager::getInstance().loadTexture(path, path);
            _texture = graphics::AssetManager::getInstance().getTexture(path);
        }
        if (_texture && _texture->valid()) {
            _size = {static_cast<float>(_texture->width()), static_cast<float>(_texture->height())};
        }
    } catch (const std::exception& e) {
        std::cerr << "[UIImage] Error loading texture: " << e.what() << std::endl;
    }
}

void UIImage::draw() {
    if (_isVisible && _texture && _texture->valid()) {
        _texture->draw(_position, 0.0F, _scale, _tint);
    }
}

void UIImage::update() {}

void UIImage::handleEvent(const raylib::rcore::Event& /*event*/) {}

void UIImage::setPosition(float x, float y) { _position = {x, y}; }

void UIImage::setSize(float width, float height) {
    _size = {width, height};
    if (_texture && _texture->valid() && _texture->width() > 0) {
        _scale = width / static_cast<float>(_texture->width());
    }
}

bool UIImage::isVisible() const { return _isVisible; }

void UIImage::setVisible(bool visible) { _isVisible = visible; }

void UIImage::setTint(raylib::Color color) { _tint = color; }

void UIImage::setScale(float scale) {
    _scale = scale;
    if (_texture && _texture->valid()) {
        _size = {static_cast<float>(_texture->width()) * _scale, static_cast<float>(_texture->height()) * _scale};
    }
}

void UIImage::setTexture(const std::string& path) {
    try {
        if (path.empty()) {
            return;
        }
        graphics::AssetManager::getInstance().loadTexture(path, path);
        auto newTexture = graphics::AssetManager::getInstance().getTexture(path);
        if (!newTexture || !newTexture->valid()) {
            std::cerr << "[UIImage] Error loading texture: Invalid texture" << std::endl;
            return;
        }
        _texture = newTexture;
        _path = path;
        _size = {static_cast<float>(_texture->width()), static_cast<float>(_texture->height())};
        if (_texture->width() > 0) {
            _scale = _size.x() / static_cast<float>(_texture->width());
        }
    } catch (const std::exception& e) {
        std::cerr << "[UIImage] Error loading texture: " << e.what() << std::endl;
    }
}

}  // namespace zappy::gui::ui::components
