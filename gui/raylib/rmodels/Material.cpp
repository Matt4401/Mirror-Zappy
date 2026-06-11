/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Material
*/

#include "Material.hpp"

#include <raylib.h>

#include <cstddef>
#include <memory>
#include <span>
#include <string>

#include "rtextures/Texture2D.hpp"

namespace zappy::gui::raylib::rmodels {
Material::Material(const std::string& path) {
    int count = 0;

    const std::unique_ptr<::Material, decltype(&MemFree)> loadedMaterials{LoadMaterials(path.c_str(), &count),
                                                                          &MemFree};
    if (!loadedMaterials || count <= 0) {
        return;
    }
    const std::span<const ::Material> materials{loadedMaterials.get(), static_cast<std::size_t>(count)};
    _materials.assign(materials.begin(), materials.end());
}

void Material::reset() {
    for (auto& material : _materials) {
        UnloadMaterial(material);
    }
    _materials.clear();
}

void Material::setTexture(int mapType, const rtextures::Texture2D& texture) {
    for (auto& material : _materials) {
        SetMaterialTexture(&material, mapType, texture.texture());
    }
}
}  // namespace zappy::gui::raylib::rmodels
