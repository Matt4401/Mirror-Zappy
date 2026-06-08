/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Sound
*/

#pragma once
#include <raylib.h>

#include <span>
#include <string>
#include <utility>

namespace zappy::gui::raylib::raudio {
class Sound {
  public:
    explicit Sound(const std::string& path) : _sound{LoadSound(path.c_str())} {}
    explicit Sound(::Sound sound) : _sound{sound} {}

    ~Sound() { reset(); }

    Sound(const Sound& other) = delete;
    Sound& operator=(const Sound& other) = delete;

    Sound(Sound&& other) noexcept : _sound{std::exchange(other._sound, {})} {}

    Sound& operator=(Sound&& other) noexcept {
        if (this != &other) {
            reset();
            _sound = std::exchange(other._sound, {});
        }
        return *this;
    }

    [[nodiscard]] bool valid() const { return IsSoundValid(_sound); }
    [[nodiscard]] bool playing() const { return IsSoundPlaying(_sound); }

    void play() const { PlaySound(_sound); }
    void stop() const { StopSound(_sound); }
    void pause() const { PauseSound(_sound); }
    void resume() const { ResumeSound(_sound); }

    void setVolume(float volume) const { SetSoundVolume(_sound, volume); }
    void setPitch(float pitch) const { SetSoundPitch(_sound, pitch); }
    void setPan(float pan) const { SetSoundPan(_sound, pan); }

    void update(std::span<const float> samples, int frameCount) { UpdateSound(_sound, samples.data(), frameCount); }

  protected:
  private:
    void reset() {
        if (valid()) {
            UnloadSound(_sound);
        }
        _sound = {};
    }

    ::Sound _sound{};
};
}  // namespace zappy::gui::raylib::raudio
