/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Model
*/

#pragma once
#include <raylib.h>

#include <string>
#include <utility>

#include "rcamera.h"
#include "rmodels/Mesh.hpp"

namespace zappy::gui::raylib::rmodels {
class Model {
  public:
    explicit Model(const std::string& path) : _model{LoadModel(path.c_str())} {}
    explicit Model(Mesh&& mesh) : _model{LoadModelFromMesh(std::move(mesh).release())} {}
    explicit Model(::Model model) : _model{model} {}

    ~Model() { reset(); }

    Model(const Model& other) = delete;
    Model& operator=(const Model& other) = delete;

    Model(Model&& other) noexcept : _model{std::exchange(other._model, {})} {}

    Model& operator=(Model&& other) noexcept {
        if (this != &other) {
            reset();
            _model = std::exchange(other._model, {});
        }
        return *this;
    }

    [[nodiscard]] bool valid() const { return IsModelValid(_model); }
    [[nodiscard]] ::Model model() const { return _model; }
    [[nodiscard]] ::Model& model() { return _model; }

    void drawModel(Vector3 position, float scale, Color tint) const { DrawModel(_model, position, scale, tint); }
    void drawModelEx(Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint) const {
        DrawModelEx(_model, position, rotationAxis, rotationAngle, scale, tint);
    }
    void drawModelWires(Vector3 position, float scale, Color tint) const {
        DrawModelWires(_model, position, scale, tint);
    }
    void drawModelWiresEx(Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale,
                          Color tint) const {
        DrawModelWiresEx(_model, position, rotationAxis, rotationAngle, scale, tint);
    }

  protected:
  private:
    void reset() {
        if (valid()) {
            UnloadModel(_model);
        }
        _model = {};
    }

    ::Model _model{};
};
}  // namespace zappy::gui::raylib::rmodels
