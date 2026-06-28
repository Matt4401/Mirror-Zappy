/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** FloatingIsland
*/

#include "FloatingIsland.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <span>
#include <vector>

#include "Color.hpp"
#include "Tile3D.hpp"
#include "events/EventDispatcher.hpp"
#include "protocol/Commands.hpp"
#include "rcore/Camera.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Mesh.hpp"
#include "rtextures/Image.hpp"
#include "scene/WorldManager.hpp"

namespace zappy::gui::graphics::scene {

FloatingIsland::FloatingIsland(WorldManager& worldManager, events::EventDispatcher& dispatcher)
    : _worldManager(worldManager), _dispatcher(dispatcher) {
    _textures.emplace(IslandBlockType::Dirt, "assets/models/environment/island/dirt.png");
    _textures.emplace(IslandBlockType::RootedDirt, "assets/models/environment/island/rooted_dirt.png");
    _textures.emplace(IslandBlockType::Stone, "assets/models/environment/island/stone.png");
    _textures.emplace(IslandBlockType::Cobblestone, "assets/models/environment/island/cobblestone.png");
    _textures.emplace(IslandBlockType::MossyCobblestone, "assets/models/environment/island/mossy_cobblestone.png");
    _textures.emplace(IslandBlockType::Deepslate, "assets/models/environment/island/deepslate.png");

    _mszToken = _dispatcher.get().subscribe<shared::protocol::server::Msz>(
        [this](const shared::protocol::server::Msz& /*e*/) { generateIsland(); });
}

FloatingIsland::~FloatingIsland() {
    if (_mszToken != 0) {
        _dispatcher.get().unsubscribe<shared::protocol::server::Msz>(_mszToken);
    }
}

void FloatingIsland::generateIsland() {
    _blocks.clear();

    const int width = _worldManager.get().width();
    const int height = _worldManager.get().height();

    if (width <= 0 || height <= 0) {
        return;
    }

    const float maxDepth = static_cast<float>(std::max(width, height)) * MaxDepthFactor;
    const float offsetX = static_cast<float>(width - 1) * Tile3D::TILE_SIZE * CenterOffsetMultiplier;
    const float offsetZ = static_cast<float>(height - 1) * Tile3D::TILE_SIZE * CenterOffsetMultiplier;

    std::vector<std::vector<int>> depthGrid(height, std::vector<int>(width, 0));
    std::vector<std::vector<unsigned char>> noiseGrid(height, std::vector<unsigned char>(width, 0));

    buildDepthGrid(width, height, maxDepth, depthGrid, noiseGrid);
    buildBlocks(width, height, offsetX, offsetZ, depthGrid, noiseGrid);

    _model.emplace(raylib::rmodels::Mesh::genCube(Tile3D::TILE_SIZE, Tile3D::TILE_SIZE, Tile3D::TILE_SIZE));
}

void FloatingIsland::buildDepthGrid(int width, int height, float maxDepth, std::vector<std::vector<int>>& depthGrid,
                                    std::vector<std::vector<unsigned char>>& noiseGrid) {
    auto noiseImage = raylib::rtextures::Image::genPerlinNoise(width, height, GetRandomValue(0, PerlinNoiseMaxSeed),
                                                               GetRandomValue(0, PerlinNoiseMaxSeed), PerlinNoiseScale);
    ImageFormat(&noiseImage.image(), PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    std::span<::Color> const pixels{static_cast<::Color*>(noiseImage.image().data),
                                    static_cast<std::size_t>(width * height)};

    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            const float halfW = std::max((static_cast<float>(width) - 1.0F) / 2.0F, MinHalfSize);
            const float halfH = std::max((static_cast<float>(height) - 1.0F) / 2.0F, MinHalfSize);
            const float dx = (static_cast<float>(x) - halfW) / halfW;
            const float dz = (static_cast<float>(z) - halfH) / halfH;

            const float dist = std::max(std::abs(dx), std::abs(dz));
            float mask = 1.0F - std::pow(dist, DistPower);
            mask = std::max(mask, 0.0F);

            unsigned char const noiseVal = pixels[((z * width) + x)].r;
            float const rawHeight = (static_cast<float>(noiseVal) / PixelNormalization) * maxDepth * mask;

            depthGrid.at(z).at(x) = std::max(1, static_cast<int>(std::ceil(rawHeight)));
            noiseGrid.at(z).at(x) = noiseVal;
        }
    }
}

void FloatingIsland::buildBlocks(int width, int height, float offsetX, float offsetZ,
                                 const std::vector<std::vector<int>>& depthGrid,
                                 const std::vector<std::vector<unsigned char>>& noiseGrid) {
    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            int const blocksDepth = depthGrid.at(z).at(x);
            unsigned char const noiseVal = noiseGrid.at(z).at(x);

            for (int y = -1; y >= -blocksDepth; y--) {
                if (!isBlockExposed(x, y, z, width, height, blocksDepth, depthGrid)) {
                    continue;
                }

                raylib::rmath::Vector3 const pos{(static_cast<float>(x) * Tile3D::TILE_SIZE) - offsetX,
                                                 (static_cast<float>(y) * Tile3D::TILE_SIZE) + YOffset,
                                                 (static_cast<float>(z) * Tile3D::TILE_SIZE) - offsetZ};

                IslandBlockType const type = determineBlockType(y, noiseVal);
                _blocks[type].push_back(pos);
            }
        }
    }
}

bool FloatingIsland::isBlockExposed(int x, int y, int z, int width, int height, int blocksDepth,
                                    const std::vector<std::vector<int>>& depthGrid) {
    if (y == -1 || y == -blocksDepth || x == 0 || x == width - 1 || z == 0 || z == height - 1) {
        return true;
    }
    int const cd = -y;
    return depthGrid.at(z).at(x - 1) < cd || depthGrid.at(z).at(x + 1) < cd || depthGrid.at(z - 1).at(x) < cd ||
           depthGrid.at(z + 1).at(x) < cd;
}

IslandBlockType FloatingIsland::determineBlockType(int y, unsigned char noiseVal) {
    if (y == -1) {
        return IslandBlockType::Dirt;
    }
    if (y == RootedDirtDepth) {
        return IslandBlockType::RootedDirt;
    }
    if (y > CobblestoneDepth) {
        return IslandBlockType::Cobblestone;
    }

    auto const rockVal = static_cast<unsigned char>((noiseVal + std::abs(y) * NoiseRockMultiplier) %
                                                    static_cast<int>(PixelNormalization));
    if (rockVal < NoiseDeepslateThreshold) {
        return IslandBlockType::Deepslate;
    }
    if (rockVal < NoiseStoneThreshold) {
        return IslandBlockType::Stone;
    }

    return IslandBlockType::MossyCobblestone;
}

void FloatingIsland::draw(const raylib::rcore::Camera& camera) const {
    if (_model) {
        for (const auto& [type, positions] : _blocks) {
            _model->setMaterialTexture(0, MATERIAL_MAP_ALBEDO, _textures.at(type));
            for (const auto& pos : positions) {
                if (camera.isVisibleFromCamera(pos)) {
                    _model->drawModel(pos, 1.0F, raylib::Color::White());
                }
            }
        }
    }
}
}  // namespace zappy::gui::graphics::scene
