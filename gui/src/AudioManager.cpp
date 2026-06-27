/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** AudioManager
*/

#include "AudioManager.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <string_view>

#include "raudio/Music.hpp"
#include "raudio/Sound.hpp"
#include "rmath/Vector3.hpp"

namespace zappy::gui {
AudioManager::AudioManager() {
    initAudioDevice();
    if (_audioReady) {
        loadMusic(DefaultBackgroundMusicPath);
        loadSound("walk", SoundWalkPath, DefaultMusicVolume, DefaultTicks);
        loadSound("levelup", SoundLevelUpPath);
        loadSound("incantation", SoundIncantationPath, DefaultMusicVolume, IncantationActionTicks);
        loadSound("item", SoundDropItemPath, DefaultMusicVolume, DefaultTicks);
        loadSound("death", SoundPlayerDeathPath, DefaultMusicVolume);
        loadSound("fork", SoundPlayerForkPath, DefaultMusicVolume, DefaultTicks);
        loadSound("eject", SoundPlayerEjectPath, DefaultMusicVolume);
        loadSound("connected", SoundPlayerConnectedPath, DefaultMusicVolume, DefaultTicks);
        loadSound("fork_anim", SoundPlayerForkAnimPath, DefaultMusicVolume, ForkActionTicks);
        loadSound("button_click", SoundButtonClickPath, DefaultMusicVolume, DefaultTicks, false);
        playMusic();
    }
}

AudioManager::~AudioManager() { closeAudioDevice(); }

void AudioManager::initAudioDevice() {
    _ownsAudioDevice = !raylib::raudio::Music::isAudioDeviceReady();
    if (_ownsAudioDevice) {
        raylib::raudio::Music::initAudioDevice();
    }
    _audioReady = raylib::raudio::Music::isAudioDeviceReady();
}

void AudioManager::closeAudioDevice() {
    _activeSounds.clear();
    _sounds.clear();
    _soundActionTicks.clear();
    _fixedPitchSounds.clear();
    _backgroundMusic.reset();
    if (_ownsAudioDevice && raylib::raudio::Music::isAudioDeviceReady()) {
        raylib::raudio::Music::closeAudioDevice();
    }
    _audioReady = false;
    _ownsAudioDevice = false;
}

bool AudioManager::hasSound(const std::string_view id) const { return _sounds.contains(std::string{id}); }

void AudioManager::update() {
    if (_backgroundMusic && _backgroundMusic->valid()) {
        _backgroundMusic->update();
        _backgroundMusic->setPitch(frequencyPitch());
    }
    for (auto& activeSound : _activeSounds) {
        if (const auto sound = _sounds.find(activeSound.id); sound != _sounds.end() && sound->second.valid()) {
            activeSound.sound.setPitch(playbackPitch(activeSound.id, sound->second));
        }
        if (activeSound.position.has_value() && activeSound.sound.playing()) {
            activeSound.sound.setVolume(spatialVolume(activeSound.position.value()));
            activeSound.sound.setPan(spatialPan(activeSound.position.value()));
        }
    }
    std::erase_if(_activeSounds, [](const ActiveSound& activeSound) { return !activeSound.sound.playing(); });
}

void AudioManager::loadMusic(const std::string& path, const float volume, const bool looping) {
    if (!_audioReady) {
        return;
    }
    _musicVolume = std::clamp(volume, 0.0F, 1.0F);
    _backgroundMusic = std::make_unique<raylib::raudio::Music>(path, _musicVolume);
    _backgroundMusic->setLooping(looping);
}

void AudioManager::playMusic() const {
    if (_backgroundMusic && _backgroundMusic->valid() && !_backgroundMusic->playing()) {
        _backgroundMusic->play();
    }
}

void AudioManager::stopMusic() const {
    if (_backgroundMusic && _backgroundMusic->valid()) {
        _backgroundMusic->stop();
    }
}

void AudioManager::setMusicVolume(const float volume) {
    _musicVolume = std::clamp(volume, 0.0F, 1.0F);
    if (_backgroundMusic && _backgroundMusic->valid()) {
        _backgroundMusic->setVolume(_musicVolume);
    }
}

void AudioManager::setSoundVolume(const float volume) {
    _soundVolume = std::clamp(volume, 0.0F, 1.0F);
    for (auto& activeSound : _activeSounds) {
        if (activeSound.position.has_value()) {
            activeSound.sound.setVolume(spatialVolume(activeSound.position.value()));
        } else {
            activeSound.sound.setVolume(_soundVolume);
        }
    }
}

void AudioManager::loadSound(const std::string_view id, const std::string& path, const float volume,
                             const float actionTicks, const bool scaleWithServerFrequency) {
    if (!_audioReady) {
        return;
    }
    const std::string key{id};
    stopSound(key);
    _sounds.erase(key);
    _sounds.emplace(key, raylib::raudio::Sound{path, volume});
    if (actionTicks > 0.0F) {
        _soundActionTicks[key] = actionTicks;
    } else {
        _soundActionTicks.erase(key);
    }
    if (scaleWithServerFrequency) {
        _fixedPitchSounds.erase(key);
    } else {
        _fixedPitchSounds.insert(key);
    }
}

void AudioManager::playSound(const std::string_view id) {
    if (const auto sound = _sounds.find(std::string{id}); sound != _sounds.end() && sound->second.valid()) {
        auto& activeSound =
            _activeSounds.emplace_back(std::string{id}, raylib::raudio::Sound::aliasFrom(sound->second));
        activeSound.sound.setVolume(_soundVolume);
        activeSound.sound.setPitch(playbackPitch(id, sound->second));
        activeSound.sound.play();
    }
}

void AudioManager::playSoundAt(const std::string_view id, const raylib::rmath::Vector3& position) {
    if (const auto sound = _sounds.find(std::string{id}); sound != _sounds.end() && sound->second.valid()) {
        auto& activeSound =
            _activeSounds.emplace_back(std::string{id}, raylib::raudio::Sound::aliasFrom(sound->second), position);
        activeSound.sound.setVolume(spatialVolume(position));
        activeSound.sound.setPan(spatialPan(position));
        activeSound.sound.setPitch(playbackPitch(id, sound->second));
        activeSound.sound.play();
    }
}

void AudioManager::stopSound(const std::string_view id) {
    if (const auto sound = _sounds.find(std::string{id}); sound != _sounds.end() && sound->second.valid()) {
        sound->second.stop();
    }
    std::erase_if(_activeSounds, [id](const ActiveSound& activeSound) {
        if (activeSound.id != id) {
            return false;
        }
        activeSound.sound.stop();
        return true;
    });
}

void AudioManager::setListener(const raylib::rmath::Vector3& position, const raylib::rmath::Vector3& target) {
    _listenerPosition = position;
    const auto forward = target - position;
    if (forward.lengthSquared() > 0.0F) {
        _listenerForward = forward.normalized();
    }
}

void AudioManager::setServerFrequency(const int frequency) {
    _serverFrequency = std::max(frequency, MinServerFrequency);
}

float AudioManager::spatialVolume(const raylib::rmath::Vector3& position) const {
    const auto distance = _listenerPosition.distance(position);
    if (distance <= SpatialFullVolumeDistance) {
        return _soundVolume * MaxSpatialVolume;
    }
    if (distance >= SpatialMaxDistance) {
        return _soundVolume * MinSpatialVolume;
    }
    const auto range = SpatialMaxDistance - SpatialFullVolumeDistance;
    const auto amount = (distance - SpatialFullVolumeDistance) / range;
    return _soundVolume * (MaxSpatialVolume - ((MaxSpatialVolume - MinSpatialVolume) * amount * amount));
}

float AudioManager::spatialPan(const raylib::rmath::Vector3& position) const {
    static constexpr float CenterPan = 0.5F;
    const auto toSound = position - _listenerPosition;
    if (toSound.lengthSquared() <= 0.0F) {
        return CenterPan;
    }
    const raylib::rmath::Vector3 up{0.0F, 1.0F, 0.0F};
    const auto right = up.cross(_listenerForward).normalized();
    const auto side = std::clamp(toSound.dot(right) / SpatialPanDistance, -1.0F, 1.0F);
    return std::clamp(CenterPan + (side * CenterPan), 0.0F, 1.0F);
}

float AudioManager::frequencyPitch() const {
    const auto frequency = static_cast<float>(std::max(_serverFrequency, MinServerFrequency));
    if (frequency <= static_cast<float>(DefaultServerFrequency)) {
        const auto range = static_cast<float>(DefaultServerFrequency - MinServerFrequency);
        const auto amount = std::clamp((frequency - static_cast<float>(MinServerFrequency)) / range, 0.0F, 1.0F);
        return MinSoundPitch + ((NormalSoundPitch - MinSoundPitch) * amount);
    }
    const auto range = static_cast<float>(FastServerFrequency - DefaultServerFrequency);
    const auto amount = std::clamp((frequency - static_cast<float>(DefaultServerFrequency)) / range, 0.0F, 1.0F);
    return NormalSoundPitch + ((MaxSoundPitch - NormalSoundPitch) * amount);
}

float AudioManager::playbackPitch(const std::string_view id, const raylib::raudio::Sound& sound) const {
    const std::string key{id};
    if (_fixedPitchSounds.contains(key)) {
        return NormalSoundPitch;
    }
    const auto ticks = _soundActionTicks.find(key);
    if (ticks == _soundActionTicks.end()) {
        return frequencyPitch();
    }
    const auto soundDuration = sound.duration();
    const auto targetDuration = ticks->second / static_cast<float>(std::max(_serverFrequency, MinServerFrequency));

    return std::clamp(soundDuration / targetDuration, MinSoundPitch, MaxSoundPitch);
}
}  // namespace zappy::gui
