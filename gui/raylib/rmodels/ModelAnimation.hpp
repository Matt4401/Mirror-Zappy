/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** ModelAnimation
*/

#pragma once
#include <raylib.h>

#include <memory>
#include <string>
#include <utility>

#include "Model.hpp"

namespace zappy::gui::raylib::rmodels {
class ModelAnimation {
  public:
    explicit ModelAnimation(const std::string& file)
        : _animations{LoadModelAnimations(file.c_str(), &_animationCount)} {}

    ~ModelAnimation() { reset(); }

    ModelAnimation(const ModelAnimation& other) = delete;
    ModelAnimation& operator=(const ModelAnimation& other) = delete;

    ModelAnimation(ModelAnimation&& other) noexcept
        : _animations{std::exchange(other._animations, nullptr)},
          _animationCount{std::exchange(other._animationCount, 0)} {}

    ModelAnimation& operator=(ModelAnimation&& other) noexcept {
        if (this != &other) {
            reset();
            _animations = std::exchange(other._animations, nullptr);
            _animationCount = std::exchange(other._animationCount, 0);
        }
        return *this;
    }

    [[nodiscard]] bool valid() const { return _animations != nullptr && _animationCount > 0; }
    [[nodiscard]] int frameCount() const { return valid() ? _animations->keyframeCount : 0; }
    [[nodiscard]] bool compatibleWith(const rmodels::Model& model) const {
        return valid() && model.valid() && IsModelAnimationValid(model.model(), *_animations);
    }

    void update(rmodels::Model& model, float frame) const {
        if (compatibleWith(model)) {
            UpdateModelAnimation(model.model(), *_animations, frame);
        }
    }

  protected:
  private:
    void reset() {
        if (_animations != nullptr) {
            UnloadModelAnimations(_animations.get(), _animationCount);
        }
        _animationCount = 0;
    }

    std::unique_ptr<::ModelAnimation> _animations = nullptr;
    int _animationCount = 0;
};
}  // namespace zappy::gui::raylib::rmodels
