/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Wave
*/

#pragma once
#include <raylib.h>

#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "raudio/Sound.hpp"

namespace zappy::gui::raylib::raudio {
class Wave {
  public:
    explicit Wave(const std::string& path) : _wave{LoadWave(path.c_str())} {}
    explicit Wave(::Wave wave) : _wave{wave} {}

    ~Wave() { reset(); }

    Wave(const Wave& other) = delete;
    Wave& operator=(const Wave& other) = delete;

    Wave(Wave&& other) noexcept : _wave{std::exchange(other._wave, {})} {}

    Wave& operator=(Wave&& other) noexcept {
        if (this != &other) {
            reset();
            _wave = std::exchange(other._wave, {});
        }
        return *this;
    }

    [[nodiscard]] static Wave fromMemory(std::string_view fileType, std::span<const unsigned char> data);

    [[nodiscard]] bool valid() const { return IsWaveValid(_wave); }
    [[nodiscard]] bool exportTo(const std::string& path) const { return ExportWave(_wave, path.c_str()); }
    [[nodiscard]] Sound toSound() const { return Sound{LoadSoundFromWave(_wave)}; }

    void crop(int firstFrame, int lastFrame) { WaveCrop(&_wave, firstFrame, lastFrame); }
    void format(int sampleRate, int sampleSize, int channels) { WaveFormat(&_wave, sampleRate, sampleSize, channels); }

  protected:
  private:
    void reset();

    ::Wave _wave{};
};
}  // namespace zappy::gui::raylib::raudio
