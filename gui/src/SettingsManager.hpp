/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** SettingsManager
*/

#pragma once

#include <raylib.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace zappy::gui {

struct SettingsData {
    float masterVolume{1.0F};
    float musicVolume{1.0F};
    float sfxVolume{1.0F};
    bool muted{false};

    int displayMode{0};
    int resolutionIndex{0};
    float fov{60.0F};
    int fpsLimit{60};
    float cameraSensitivity{1.0F};
    float cameraSpeed{6.0F};
    int renderDistance{100};

    bool showUI{true};
    bool showPlayers{true};
    bool showMinerals{true};
    bool showFood{true};
    bool showTiles{true};

    std::map<std::string, int> keybinds{
        {"Move Forward", KEY_W}, {"Move Backward", KEY_S},      {"Move Left", KEY_A},  {"Move Right", KEY_D},
        {"Move Up", KEY_SPACE},  {"Move Down", KEY_LEFT_SHIFT}, {"Pause", KEY_ESCAPE}, {"Sprint", KEY_LEFT_CONTROL},
        {"Reset Camera", KEY_R}, {"Toggle UI", KEY_LEFT_ALT}};
};

class SettingsManager {
  public:
    SettingsManager(std::string configFilePath);
    ~SettingsManager() = default;

    SettingsManager(const SettingsManager& other) = delete;
    SettingsManager& operator=(const SettingsManager& other) = delete;
    SettingsManager(SettingsManager&& other) = delete;
    SettingsManager& operator=(SettingsManager&& other) = delete;

    void load();
    void save() const;

    SettingsData& getSettings();
    [[nodiscard]] const SettingsData& getSettings() const;

  private:
    struct LoadProp {
        std::string key;
        std::function<void(const std::string&)> setter;
    };
    struct SaveProp {
        std::string section;
        std::string key;
        std::function<std::string()> getter;
    };
    std::vector<LoadProp> getLoadProperties();
    std::vector<SaveProp> getSaveProperties() const;

    std::string _configFilePath;
    SettingsData _settings;
};

}  // namespace zappy::gui
