/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** SettingsManager
*/

#include "SettingsManager.hpp"

#include <cstdio>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace zappy::gui {

SettingsManager::SettingsManager(std::string configFilePath) : _configFilePath(std::move(configFilePath)) { load(); }

SettingsData& SettingsManager::getSettings() { return _settings; }

[[nodiscard]] const SettingsData& SettingsManager::getSettings() const { return _settings; }

std::vector<SettingsManager::LoadProp> SettingsManager::getLoadProperties() {
    return {
        {.key = "masterVolume", .setter = [this](const std::string& v) { _settings.masterVolume = std::stof(v); }},
        {.key = "musicVolume", .setter = [this](const std::string& v) { _settings.musicVolume = std::stof(v); }},
        {.key = "sfxVolume", .setter = [this](const std::string& v) { _settings.sfxVolume = std::stof(v); }},
        {.key = "muted", .setter = [this](const std::string& v) { _settings.muted = (v == "1" || v == "true"); }},
        {.key = "displayMode", .setter = [this](const std::string& v) { _settings.displayMode = std::stoi(v); }},
        {.key = "resolutionIndex",
         .setter = [this](const std::string& v) { _settings.resolutionIndex = std::stoi(v); }},
        {.key = "fov", .setter = [this](const std::string& v) { _settings.fov = std::stof(v); }},
        {.key = "fpsLimit", .setter = [this](const std::string& v) { _settings.fpsLimit = std::stoi(v); }},
        {.key = "cameraSensitivity",
         .setter = [this](const std::string& v) { _settings.cameraSensitivity = std::stof(v); }},
        {.key = "cameraSpeed", .setter = [this](const std::string& v) { _settings.cameraSpeed = std::stof(v); }},
        {.key = "renderDistance", .setter = [this](const std::string& v) { _settings.renderDistance = std::stoi(v); }},
        {.key = "showUI", .setter = [this](const std::string& v) { _settings.showUI = (v == "1" || v == "true"); }},
        {.key = "showPlayers",
         .setter = [this](const std::string& v) { _settings.showPlayers = (v == "1" || v == "true"); }},
        {.key = "showMinerals",
         .setter = [this](const std::string& v) { _settings.showMinerals = (v == "1" || v == "true"); }},
        {.key = "showFood", .setter = [this](const std::string& v) { _settings.showFood = (v == "1" || v == "true"); }},
        {.key = "showTiles",
         .setter = [this](const std::string& v) { _settings.showTiles = (v == "1" || v == "true"); }},
        {.key = "showFloatingIsland",
         .setter = [this](const std::string& v) { _settings.showFloatingIsland = (v == "1" || v == "true"); }}};
}

std::vector<SettingsManager::SaveProp> SettingsManager::getSaveProperties() const {
    return {
        {.section = "Audio",
         .key = "masterVolume",
         .getter = [this]() { return std::to_string(_settings.masterVolume); }},
        {.section = "Audio",
         .key = "musicVolume",
         .getter = [this]() { return std::to_string(_settings.musicVolume); }},
        {.section = "Audio", .key = "sfxVolume", .getter = [this]() { return std::to_string(_settings.sfxVolume); }},
        {.section = "Audio", .key = "muted", .getter = [this]() { return _settings.muted ? "1" : "0"; }},
        {.section = "Video",
         .key = "displayMode",
         .getter = [this]() { return std::to_string(_settings.displayMode); }},
        {.section = "Video",
         .key = "resolutionIndex",
         .getter = [this]() { return std::to_string(_settings.resolutionIndex); }},
        {.section = "Video", .key = "fov", .getter = [this]() { return std::to_string(_settings.fov); }},
        {.section = "Video", .key = "fpsLimit", .getter = [this]() { return std::to_string(_settings.fpsLimit); }},
        {.section = "Video",
         .key = "cameraSensitivity",
         .getter = [this]() { return std::to_string(_settings.cameraSensitivity); }},
        {.section = "Video",
         .key = "cameraSpeed",
         .getter = [this]() { return std::to_string(_settings.cameraSpeed); }},
        {.section = "Video",
         .key = "renderDistance",
         .getter = [this]() { return std::to_string(_settings.renderDistance); }},
        {.section = "Visibility", .key = "showUI", .getter = [this]() { return _settings.showUI ? "1" : "0"; }},
        {.section = "Visibility",
         .key = "showPlayers",
         .getter = [this]() { return _settings.showPlayers ? "1" : "0"; }},
        {.section = "Visibility",
         .key = "showMinerals",
         .getter = [this]() { return _settings.showMinerals ? "1" : "0"; }},
        {.section = "Visibility", .key = "showFood", .getter = [this]() { return _settings.showFood ? "1" : "0"; }},
        {.section = "Visibility", .key = "showTiles", .getter = [this]() { return _settings.showTiles ? "1" : "0"; }},
        {.section = "Visibility",
         .key = "showFloatingIsland",
         .getter = [this]() { return _settings.showFloatingIsland ? "1" : "0"; }},
    };
}

void SettingsManager::load() {
    std::ifstream file(_configFilePath);
    if (!file.is_open()) {
        std::cerr << "SettingsManager: Could not open config file " << _configFilePath
                  << " for reading. Using defaults." << std::endl;
        return;
    }

    auto props = getLoadProperties();
    std::unordered_map<std::string, std::function<void(const std::string&)>> setters;
    for (const auto& p : props) {
        setters[p.key] = p.setter;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line.at(0) == '#' || line.at(0) == '[') {
            continue;
        }

        std::istringstream iss(line);
        std::string key;
        std::string valueStr;

        if (std::getline(iss, key, '=') && std::getline(iss, valueStr)) {
            try {
                if (key.starts_with("bind_")) {
                    _settings.keybinds.at(key.substr(5)) = std::stoi(valueStr);
                } else if (key.starts_with("ui_")) {
                    std::string const id = key.substr(3);
                    std::vector<int> layout;
                    std::stringstream ss2(valueStr);
                    std::string token;
                    while (std::getline(ss2, token, ',')) {
                        layout.push_back(std::stoi(token));
                    }
                    if (layout.size() == 4) {
                        _settings.uiLayouts[id] = layout;
                    }
                } else if (auto it = setters.find(key); it != setters.end()) {
                    it->second(valueStr);
                }
            } catch (const std::exception& e) {
                std::cerr << "SettingsManager: Error parsing key '" << key << "' with value '" << valueStr
                          << "': " << e.what() << std::endl;
            }
        }
    }
}

void SettingsManager::save() const {
    const std::string tmpPath = _configFilePath + ".tmp";
    std::ofstream file(tmpPath);
    if (!file.is_open()) {
        std::cerr << "SettingsManager: Could not open config file " << tmpPath << " for writing." << std::endl;
        return;
    }

    auto props = getSaveProperties();
    std::string currentSection;

    for (const auto& p : props) {
        if (p.section != currentSection) {
            currentSection = p.section;
            file << "\n[" << currentSection << "]\n";
        }
        file << p.key << "=" << p.getter() << "\n";
    }

    file << "\n[Controls]\n";
    for (const auto& [action, key] : _settings.keybinds) {
        file << "bind_" << action << "=" << key << "\n";
    }

    file << "\n[UILayout]\n";
    for (const auto& [id, layout] : _settings.uiLayouts) {
        if (layout.size() == 4) {
            file << "ui_" << id << "=" << layout.at(0) << "," << layout.at(1) << "," << layout.at(2) << ","
                 << layout.at(3) << "\n";
        }
    }

    file.flush();
    if (!file.good()) {
        std::cerr << "SettingsManager: Failed to write to config file." << std::endl;
        file.close();
        return;
    }
    file.close();

    if (std::rename(tmpPath.c_str(), _configFilePath.c_str()) != 0) {
        std::cerr << "SettingsManager: Failed to atomically replace config file." << std::endl;
    }
}

}  // namespace zappy::gui
