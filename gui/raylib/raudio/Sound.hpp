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
    explicit Sound(const std::string& path, float volume = 1.0F) : _sound{LoadSound(path.c_str())} {
        SetSoundVolume(_sound, volume);
    }
    explicit Sound(::Sound sound) : _sound{sound} {}
    Sound(::Sound sound, bool alias) : _sound{sound}, _alias(alias) {}

    ~Sound() { reset(); }

    Sound(const Sound& other) = delete;
    Sound& operator=(const Sound& other) = delete;

    Sound(Sound&& other) noexcept : _sound{std::exchange(other._sound, {})}, _alias{std::exchange(other._alias, false)} {}

    Sound& operator=(Sound&& other) noexcept {
        if (this != &other) {
            reset();
            _sound = std::exchange(other._sound, {});
            _alias = std::exchange(other._alias, false);
        }
        return *this;
    }

    [[nodiscard]] static Sound aliasFrom(const Sound& sound) { return Sound{LoadSoundAlias(sound._sound), true}; }

    [[nodiscard]] bool valid() const { return IsSoundValid(_sound); }
    [[nodiscard]] bool playing() const { return IsSoundPlaying(_sound); }
    [[nodiscard]] float duration() const {
        if (!valid() || _sound.stream.sampleRate <= 0) {
            return 0.0F;
        }
        return static_cast<float>(_sound.frameCount) / static_cast<float>(_sound.stream.sampleRate);
    }

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
            if (_alias) {
                UnloadSoundAlias(_sound);
            } else {
                UnloadSound(_sound);
            }
        }
        _sound = {};
        _alias = false;
    }

    ::Sound _sound{};
    bool _alias{false};
};
}  // namespace zappy::gui::raylib::raudio
