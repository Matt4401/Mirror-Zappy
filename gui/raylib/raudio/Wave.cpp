/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Wave
*/

#include "Wave.hpp"

#include <raylib.h>

#include <span>
#include <string>
#include <string_view>

namespace zappy::gui::raylib::raudio {
void Wave::reset() {
    if (valid()) {
        UnloadWave(_wave);
    }
    _wave = {};
}

Wave Wave::fromMemory(std::string_view fileType, std::span<const unsigned char> data) {
    const std::string extension{fileType};
    return Wave{LoadWaveFromMemory(extension.c_str(), data.data(), static_cast<int>(data.size()))};
}
}  // namespace zappy::gui::raylib::raudio
