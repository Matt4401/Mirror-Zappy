/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Material
*/

#pragma once
#include <raylib.h>

#include <string>
#include <utility>
#include <vector>

#include "rtextures/Texture2D.hpp"

namespace zappy::gui::raylib::rmodels {
class Material {
  public:
    Material(const std::string& path);
    ~Material() { reset(); }

    Material(const Material& other) = delete;
    Material& operator=(const Material& other) = delete;

    Material(Material&& other) noexcept : _materials{std::move(other._materials)} { other._materials.clear(); }

    Material& operator=(Material&& other) noexcept {
        if (this != &other) {
            reset();
            _materials = std::move(other._materials);
            other._materials.clear();
        }
        return *this;
    }

    void setTexture(int mapType, const rtextures::Texture2D& texture);

  protected:
  private:
    void reset();
    std::vector<::Material> _materials;
};
}  // namespace zappy::gui::raylib::rmodels
