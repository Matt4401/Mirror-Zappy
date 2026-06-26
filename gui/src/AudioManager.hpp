/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** AudioManager
*/

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "raudio/Music.hpp"
#include "raudio/Sound.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui {
class AudioManager {
  public:
    static constexpr float DefaultMusicVolume = 0.5F;

    static constexpr auto DefaultBackgroundMusicPath = "assets/audio/Blind Spots.mp3";
    static constexpr auto SoundWalkPath = "assets/audio/minecraft-grass-walking-sound-effect.mp3";
    static constexpr auto SoundLevelUpPath = "assets/audio/levelup.mp3";
    static constexpr auto SoundIncantationPath = "assets/audio/ziak-villagers.mp3";
    static constexpr auto SoundDropItemPath = "assets/audio/minecraft-drop-block-sound-effect.mp3";
    static constexpr auto SoundPlayerDeathPath = "assets/audio/villager-death.mp3";
    static constexpr auto SoundPlayerForkPath = "assets/audio/fork-vilager.mp3";
    static constexpr auto SoundPlayerEjectPath = "assets/audio/minecraft-sword-swing.mp3";
    static constexpr auto SoundPlayerConnectedPath = "assets/audio/Player-connected(Guerrier - Doigby).mp3";

    static constexpr float MinSpatialVolume = 0.0F;
    static constexpr float MaxSpatialVolume = 0.15F;
    static constexpr float SpatialFullVolumeDistance = 3.0F;
    static constexpr float SpatialMaxDistance = 35.0F;
    static constexpr float SpatialPanDistance = 18.0F;

    static constexpr int DefaultServerFrequency = 100;
    static constexpr int MinServerFrequency = 1;
    static constexpr int FastServerFrequency = 500;

    static constexpr float MinSoundPitch = 0.75F;
    static constexpr float NormalSoundPitch = 1.0F;
    static constexpr float MaxSoundPitch = 1.5F;

    static constexpr float DefaultTicks = 7.0F;
    static constexpr float IncantationActionTicks = 300.0F;
    static constexpr float ForkActionTicks = 42.0F;

    AudioManager();
    ~AudioManager();

    AudioManager(const AudioManager& other) = delete;
    AudioManager& operator=(const AudioManager& other) = delete;
    AudioManager(AudioManager&& other) = delete;
    AudioManager& operator=(AudioManager&& other) = delete;

    [[nodiscard]] bool ready() const { return _audioReady; }
    [[nodiscard]] bool hasSound(std::string_view id) const;

    void update();
    void loadMusic(const std::string& path, float volume = DefaultMusicVolume, bool looping = true);
    void playMusic() const;
    void stopMusic() const;
    void setMusicVolume(float volume) const;

    void loadSound(std::string_view id, const std::string& path, float volume = DefaultMusicVolume,
                   float actionTicks = 0.0F);
    void playSound(std::string_view id);
    void playSoundAt(std::string_view id, const raylib::rmath::Vector3& position);
    void stopSound(std::string_view id);
    void setListener(const raylib::rmath::Vector3& position, const raylib::rmath::Vector3& target);
    void setServerFrequency(int frequency);

  private:
    struct ActiveSound {
        std::string id;
        raylib::raudio::Sound sound;
        std::optional<raylib::rmath::Vector3> position;
    };

    void initAudioDevice();
    void closeAudioDevice();
    [[nodiscard]] float spatialVolume(const raylib::rmath::Vector3& position) const;
    [[nodiscard]] float spatialPan(const raylib::rmath::Vector3& position) const;
    [[nodiscard]] float frequencyPitch() const;
    [[nodiscard]] float playbackPitch(std::string_view id, const raylib::raudio::Sound& sound) const;

    bool _audioReady{false};
    bool _ownsAudioDevice{false};
    std::unique_ptr<raylib::raudio::Music> _backgroundMusic;
    std::unordered_map<std::string, raylib::raudio::Sound> _sounds;
    std::unordered_map<std::string, float> _soundActionTicks;
    std::vector<ActiveSound> _activeSounds;
    raylib::rmath::Vector3 _listenerPosition{0.0F, 0.0F, 0.0F};
    raylib::rmath::Vector3 _listenerForward{0.0F, 0.0F, 1.0F};
    int _serverFrequency{DefaultServerFrequency};
};
}  // namespace zappy::gui
