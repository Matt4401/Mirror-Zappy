/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Skybox3D
*/

#include "Skybox3D.hpp"

#include <raylib.h>

#include <cmath>
#include <utility>

#include "Color.hpp"
#include "rcore/Matrix.hpp"
#include "rcore/Rlgl.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Mesh.hpp"
#include "rtextures/Image.hpp"
#include "rtextures/Texture2D.hpp"

namespace zappy::gui::graphics::scene {

Skybox3D::Skybox3D()
    : _model(raylib::rmodels::Mesh::genCube(1.0F, 1.0F, 1.0F)),
      _shader("assets/shaders/skybox.vs", "assets/shaders/skybox.fs") {
    int const environmentMapLoc = _shader.getLocation("environmentMap");
    _shader.setLocation(SHADER_LOC_MAP_CUBEMAP, environmentMapLoc);

    _model.setMaterialShader(0, _shader);

    loadCubemap();
}

Skybox3D::~Skybox3D() = default;

Skybox3D::Skybox3D(Skybox3D&& other) noexcept
    : _model{std::move(other._model)},
      _shader{std::move(other._shader)},
      _cubemap{std::move(other._cubemap)},
      _dayProgress{other._dayProgress} {}

Skybox3D& Skybox3D::operator=(Skybox3D&& other) noexcept {
    if (this != &other) {
        _model = std::move(other._model);
        _shader = std::move(other._shader);
        _cubemap = std::move(other._cubemap);
        _dayProgress = other._dayProgress;
    }
    return *this;
}

void Skybox3D::loadCubemap() {
    raylib::rtextures::Image const right("assets/images/skybox/Right.png");
    raylib::rtextures::Image const left("assets/images/skybox/Left.png");
    raylib::rtextures::Image const top("assets/images/skybox/Top.png");
    raylib::rtextures::Image const bottom("assets/images/skybox/Bottom.png");
    raylib::rtextures::Image const front("assets/images/skybox/Front.png");
    raylib::rtextures::Image const back("assets/images/skybox/Back.png");

    if (!right.valid() || !left.valid() || !top.valid() || !bottom.valid() || !front.valid() || !back.valid()) {
        return;
    }

    int const w = right.width();
    int const h = right.height();
    raylib::rtextures::Image cubemapImage = raylib::rtextures::Image::genColor(w, h * 6, raylib::Color{0, 0, 0, 0});

    cubemapImage.draw(right, {0, 0, (float)w, (float)h}, {0, 0, (float)w, (float)h}, raylib::Color{255, 255, 255, 255});
    cubemapImage.draw(left, {0, 0, (float)w, (float)h}, {0, (float)h, (float)w, (float)h},
                      raylib::Color{255, 255, 255, 255});
    cubemapImage.draw(top, {0, 0, (float)w, (float)h}, {0, (float)h * 2.0F, (float)w, (float)h},
                      raylib::Color{255, 255, 255, 255});
    cubemapImage.draw(bottom, {0, 0, (float)w, (float)h}, {0, (float)h * 3.0F, (float)w, (float)h},
                      raylib::Color{255, 255, 255, 255});
    cubemapImage.draw(front, {0, 0, (float)w, (float)h}, {0, (float)h * 4.0F, (float)w, (float)h},
                      raylib::Color{255, 255, 255, 255});
    cubemapImage.draw(back, {0, 0, (float)w, (float)h}, {0, (float)h * 5.0F, (float)w, (float)h},
                      raylib::Color{255, 255, 255, 255});

    _cubemap.emplace(raylib::rtextures::Texture2D::loadCubemap(cubemapImage, CUBEMAP_LAYOUT_LINE_VERTICAL));
    _model.setMaterialTexture(0, MATERIAL_MAP_CUBEMAP, *_cubemap);
}

void Skybox3D::update(float deltaTime, float serverFrequency) {
    float const timeMultiplier = serverFrequency / 100.0F;
    _dayProgress = std::fmod(_dayProgress + (deltaTime * DAY_SPEED * timeMultiplier), FULL_DAY);
    if (_dayProgress < 0.0F) {
        _dayProgress += FULL_DAY;
    }

    float const rotationAngle = _dayProgress * 360.0F;
    _model.setTransform(raylib::rcore::Matrix::rotateY(rotationAngle * DEG2RAD));
}

void Skybox3D::draw() const {
    raylib::rcore::Rlgl::disableBackfaceCulling();
    raylib::rcore::Rlgl::disableDepthMask();

    _model.drawModel(raylib::rmath::Vector3{0.0F, 0.0F, 0.0F}, 1.0F, currentTint());

    raylib::rcore::Rlgl::enableBackfaceCulling();
    raylib::rcore::Rlgl::enableDepthMask();
}

raylib::Color Skybox3D::currentTint() const {
    if (_dayProgress < MORNING_START) {
        return raylib::Color::lerp(NIGHT, SUNRISE, _dayProgress / MORNING_START);
    }
    if (_dayProgress < AFTERNOON_START) {
        return raylib::Color::lerp(SUNRISE, DAY, (_dayProgress - MORNING_START) / (AFTERNOON_START - MORNING_START));
    }
    if (_dayProgress < EVENING_START) {
        return DAY;
    }
    if (_dayProgress < NIGHT_START) {
        return raylib::Color::lerp(DAY, SUNSET, (_dayProgress - EVENING_START) / (NIGHT_START - EVENING_START));
    }
    return raylib::Color::lerp(SUNSET, NIGHT, (_dayProgress - NIGHT_START) / (FULL_DAY - NIGHT_START));
}

}  // namespace zappy::gui::graphics::scene
