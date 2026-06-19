/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Shader
*/

#pragma once
#include <raylib.h>

#include <span>
#include <string>
#include <utility>

namespace zappy::gui::raylib::rmodels {
class Shader {
  public:
    Shader() = default;
    Shader(const std::string& vsFile, const std::string& fsFile)
        : _shader{LoadShader(vsFile.c_str(), fsFile.c_str())} {}
    explicit Shader(::Shader shader) : _shader(shader) {}
    ~Shader() { reset(); }

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;

    Shader(Shader&& other) noexcept : _shader{std::exchange(other._shader, {})} {}
    Shader& operator=(Shader&& other) noexcept {
        if (this != &other) {
            reset();
            _shader = std::exchange(other._shader, {});
        }
        return *this;
    }

    [[nodiscard]] bool valid() const { return IsShaderValid(_shader); }
    [[nodiscard]] ::Shader shader() const { return _shader; }

    [[nodiscard]] int getLocation(const std::string& uniformName) const {
        return GetShaderLocation(_shader, uniformName.c_str());
    }

    void setLocation(int locationIndex, int loc) const {
        std::span<int> const locs{_shader.locs, 32};
        locs[locationIndex] = loc;
    }

  protected:
  private:
    void reset() {
        if (valid()) {
            UnloadShader(_shader);
            _shader = {};
        }
    }

    ::Shader _shader{};
};
}  // namespace zappy::gui::raylib::rmodels
