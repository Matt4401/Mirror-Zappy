/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Audio
*/

#pragma once
#include <raylib.h>

#include <cstddef>

namespace zappy::gui::raylib::raudio {
class Audio {
  public:
    Audio() {
        if (_instances == 0 && !IsAudioDeviceReady()) {
            InitAudioDevice();
            _deviceOwned = true;
        }
        _instances++;
    }

    ~Audio() {
        _instances--;
        if (_instances == 0 && _deviceOwned && IsAudioDeviceReady()) {
            CloseAudioDevice();
            _deviceOwned = false;
        }
    }

    Audio(const Audio& other) = delete;
    Audio& operator=(const Audio& other) = delete;
    Audio(Audio&& other) = delete;
    Audio& operator=(Audio&& other) = delete;

    [[nodiscard]] static bool ready() { return IsAudioDeviceReady(); }
    [[nodiscard]] static float masterVolume() { return GetMasterVolume(); }

    static void setMasterVolume(float volume) { SetMasterVolume(volume); }

  protected:
  private:
    inline static std::size_t _instances = 0;
    inline static bool _deviceOwned = false;
};
}  // namespace zappy::gui::raylib::raudio
