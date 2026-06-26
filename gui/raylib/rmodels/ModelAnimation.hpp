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
    ModelAnimation() = default;

    explicit ModelAnimation(const std::string& fileName) {
        int count = 0;
        ::ModelAnimation* anims = LoadModelAnimations(fileName.c_str(), &count);
        _animations.reset(anims);
        _animationCount = count;
    }

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
    [[nodiscard]] int frameCount(int animIndex = 0) const {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        return (valid() && animIndex >= 0 && animIndex < _animationCount) ? _animations.get()[animIndex].keyframeCount
                                                                          : 0;
    }
    [[nodiscard]] bool compatibleWith(const rmodels::Model& model, int animIndex = 0) const {
        return valid() && model.valid() && animIndex >= 0 && animIndex < _animationCount &&
               // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
               IsModelAnimationValid(model.model(), _animations.get()[animIndex]);
    }

    [[nodiscard]] int getAnimationIndex(const std::string& name) const {
        if (!valid()) {
            return 0;
        }
        for (int i = 0; i < _animationCount; i++) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            const char* animName = static_cast<const char*>(_animations.get()[i].name);
            if (animName != nullptr && std::string(animName) == name) {
                return i;
            }
        }
        return 0;
    }

    void update(rmodels::Model& model, float frame, int animIndex = 0) const {
        if (valid() && animIndex >= 0 && animIndex < _animationCount) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            UpdateModelAnimation(model.model(), _animations.get()[animIndex], frame);
        }
    }

  protected:
  private:
    void reset() {
        if (_animations != nullptr) {
            UnloadModelAnimations(_animations.release(), _animationCount);
        }
        _animationCount = 0;
    }

    std::unique_ptr<::ModelAnimation> _animations = nullptr;
    int _animationCount = 0;
};
}  // namespace zappy::gui::raylib::rmodels
