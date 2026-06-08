/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Material
*/

#pragma once
#include <raylib.h>

#include <cstddef>
#include <memory>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "rtextures/Texture2D.hpp"

namespace zappy::gui::raylib::rmodels {
class Material {
  public:
    Material(const std::string& path) {
        int count = 0;

        const std::unique_ptr<::Material, decltype(&MemFree)> loadedMaterials{LoadMaterials(path.c_str(), &count),
                                                                              &MemFree};
        if (!loadedMaterials || count <= 0) {
            return;
        }
        const std::span<const ::Material> materials{loadedMaterials.get(), static_cast<std::size_t>(count)};
        _materials.assign(materials.begin(), materials.end());
    }

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

    void setTexture(int mapType, const rtextures::Texture2D& texture) {
        for (auto& material : _materials) {
            SetMaterialTexture(&material, mapType, texture.texture());
        }
    }

  protected:
  private:
    void reset() {
        for (auto& material : _materials) {
            UnloadMaterial(material);
        }
        _materials.clear();
    }

    std::vector<::Material> _materials;
};
}  // namespace zappy::gui::raylib::rmodels
