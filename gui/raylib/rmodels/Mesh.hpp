/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Mesh
*/

#pragma once
#include <raylib.h>

#include <utility>

#include "rcore/Matrix.hpp"

namespace zappy::gui::raylib::rmodels {
class Mesh {
  public:
    explicit Mesh(::Mesh mesh) : _mesh{mesh}, _owned{true} {}
    Mesh() = default;
    ~Mesh() { reset(); }

    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;

    Mesh(Mesh&& other) noexcept : _mesh{std::exchange(other._mesh, {})}, _owned{std::exchange(other._owned, false)} {}

    Mesh& operator=(Mesh&& other) noexcept {
        if (this != &other) {
            reset();
            _mesh = std::exchange(other._mesh, {});
            _owned = std::exchange(other._owned, false);
        }
        return *this;
    }

    [[nodiscard]] ::Mesh mesh() const { return _mesh; }
    [[nodiscard]] ::Mesh& mesh() { return _mesh; }
    [[nodiscard]] ::Mesh release() {
        _owned = false;
        return std::exchange(_mesh, {});
    }

    void draw(::Material material, const rcore::Matrix& transform) const {
        DrawMesh(_mesh, material, transform.matrix());
    }

    static Mesh genCube(float width, float height, float length) { return Mesh{GenMeshCube(width, height, length)}; }

    void genMeshPolygonal(int sides, float radius) { replace(GenMeshPoly(sides, radius)); }
    void genMeshPlane(float width, float length, int resX, int resZ) {
        replace(GenMeshPlane(width, length, resX, resZ));
    }
    void genMeshCube(float width, float height, float length) { replace(GenMeshCube(width, height, length)); }
    void genMeshSphere(float radius, int rings, int slices) { replace(GenMeshSphere(radius, rings, slices)); }
    void genMeshHemiSphere(float radius, int rings, int slices) { replace(GenMeshHemiSphere(radius, rings, slices)); }
    void genMeshCylinder(float radius, float height, int slices) { replace(GenMeshCylinder(radius, height, slices)); }
    void genMeshTorus(float radius, float size, int radSeg, int sides) {
        replace(GenMeshTorus(radius, size, radSeg, sides));
    }
    void genMeshKnot(float radius, float size, int radSeg, int sides) {
        replace(GenMeshKnot(radius, size, radSeg, sides));
    }

  protected:
  private:
    void reset() {
        if (_owned) {
            UnloadMesh(_mesh);
        }
        _mesh = {};
        _owned = false;
    }

    void replace(::Mesh mesh) {
        reset();
        _mesh = mesh;
        _owned = true;
    }

    ::Mesh _mesh{};
    bool _owned = false;
};
}  // namespace zappy::gui::raylib::rmodels
