/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** FloatingIsland
*/

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <vector>

#include "events/EventDispatcher.hpp"
#include "rcore/Camera.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"
#include "rtextures/Texture2D.hpp"
#include "scene/WorldManager.hpp"

namespace zappy::gui::graphics::scene {

enum class IslandBlockType : std::uint8_t { Dirt, RootedDirt, Stone, Cobblestone, MossyCobblestone, Deepslate };

class FloatingIsland {
  public:
    FloatingIsland(WorldManager& worldManager, events::EventDispatcher& dispatcher);
    ~FloatingIsland();

    FloatingIsland(const FloatingIsland&) = delete;
    FloatingIsland& operator=(const FloatingIsland&) = delete;
    FloatingIsland(FloatingIsland&&) = delete;
    FloatingIsland& operator=(FloatingIsland&&) = delete;

    void draw(const raylib::rcore::Camera& camera) const;

  private:
    void generateIsland();
    static void buildDepthGrid(int width, int height, float maxDepth, std::vector<std::vector<int>>& depthGrid,
                               std::vector<std::vector<unsigned char>>& noiseGrid);
    void buildBlocks(int width, int height, float offsetX, float offsetZ,
                     const std::vector<std::vector<int>>& depthGrid,
                     const std::vector<std::vector<unsigned char>>& noiseGrid);
    [[nodiscard]] static bool isBlockExposed(int x, int y, int z, int width, int height, int blocksDepth,
                                             const std::vector<std::vector<int>>& depthGrid);
    [[nodiscard]] static IslandBlockType determineBlockType(int y, unsigned char noiseVal);

    std::reference_wrapper<WorldManager> _worldManager;
    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::optional<raylib::rmodels::Model> _model;
    std::map<IslandBlockType, std::vector<raylib::rmath::Vector3>> _blocks;
    std::map<IslandBlockType, raylib::rtextures::Texture2D> _textures;
    events::EventDispatcher::EventToken _mszToken{0};
};
}  // namespace zappy::gui::graphics::scene
