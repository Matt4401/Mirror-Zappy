/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Music
*/

#pragma once
#include <raylib.h>

#include <string>
#include <utility>

namespace zappy::gui::raylib::raudio {
class Music {
  public:
    explicit Music(const std::string& path, float volume = 0.5F) : _music{LoadMusicStream(path.c_str())} {
        SetMusicVolume(_music, volume);
    }
    explicit Music(::Music music) : _music{music} {}

    ~Music() { destroy(); }

    Music(const Music& other) = delete;
    Music& operator=(const Music& other) = delete;

    Music(Music&& other) noexcept : _music{std::exchange(other._music, {})} {}

    Music& operator=(Music&& other) noexcept {
        if (this != &other) {
            destroy();
            _music = std::exchange(other._music, {});
        }
        return *this;
    }

    [[nodiscard]] bool valid() const { return IsMusicValid(_music); }
    [[nodiscard]] bool playing() const { return IsMusicStreamPlaying(_music); }
    [[nodiscard]] float timeLength() const { return GetMusicTimeLength(_music); }
    [[nodiscard]] float timePlayed() const { return GetMusicTimePlayed(_music); }

    void play() const { PlayMusicStream(_music); }
    void stop() const { StopMusicStream(_music); }
    void pause() const { PauseMusicStream(_music); }
    void resume() const { ResumeMusicStream(_music); }
    void update() const { UpdateMusicStream(_music); }
    void seek(float position) const { SeekMusicStream(_music, position); }

    void setLooping(bool looping) { _music.looping = looping; }
    void setVolume(float volume) const { SetMusicVolume(_music, volume); }
    void setPitch(float pitch) const { SetMusicPitch(_music, pitch); }
    void setPan(float pan) const { SetMusicPan(_music, pan); }

    static bool isAudioDeviceReady() { return IsAudioDeviceReady(); }
    static void initAudioDevice() { InitAudioDevice(); }
    static void closeAudioDevice() { CloseAudioDevice(); }

  protected:
  private:
    void destroy() {
        if (valid()) {
            UnloadMusicStream(_music);
        }
        _music = {};
    }

    ::Music _music{};
};
}  // namespace zappy::gui::raylib::raudio
