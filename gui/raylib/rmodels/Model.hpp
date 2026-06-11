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

#include "Color.hpp"
#include "rmath/Vector3.hpp"
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

    void drawModel(rmath::Vector3 position, float scale, Color tint) const {
        DrawModel(_model, position.vector(), scale, tint.color());
    }
    void drawModelEx(rmath::Vector3 position, rmath::Vector3 rotationAxis, float rotationAngle, rmath::Vector3 scale,
                     Color tint) const {
        DrawModelEx(_model, position.vector(), rotationAxis.vector(), rotationAngle, scale.vector(), tint.color());
    }
    void drawModelWires(rmath::Vector3 position, float scale, Color tint) const {
        DrawModelWires(_model, position.vector(), scale, tint.color());
    }
    void drawModelWiresEx(rmath::Vector3 position, rmath::Vector3 rotationAxis, float rotationAngle,
                          rmath::Vector3 scale, Color tint) const {
        DrawModelWiresEx(_model, position.vector(), rotationAxis.vector(), rotationAngle, scale.vector(), tint.color());
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
