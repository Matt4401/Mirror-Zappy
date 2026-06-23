/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Vector2
*/

#pragma once
#include <raylib.h>
#include <raymath.h>

namespace zappy::gui::raylib::rmath {
class Vector2 {
  public:
    constexpr Vector2() = default;
    constexpr Vector2(int x, int y) : _vector{static_cast<float>(x), static_cast<float>(y)} {}
    constexpr Vector2(float x, float y) : _vector{x, y} {}
    constexpr Vector2(::Vector2 vector) : _vector{vector} {}

    [[nodiscard]] constexpr float x() const { return _vector.x; }
    [[nodiscard]] constexpr float y() const { return _vector.y; }
    [[nodiscard]] constexpr ::Vector2 vector() const { return _vector; }
    [[nodiscard]] constexpr ::Vector2& vector() { return _vector; }

    constexpr operator ::Vector2() const { return _vector; }

    constexpr void setX(float x) { _vector.x = x; }
    constexpr void setY(float y) { _vector.y = y; }

    [[nodiscard]] float length() const { return Vector2Length(_vector); }
    [[nodiscard]] float lengthSquared() const { return Vector2LengthSqr(_vector); }
    [[nodiscard]] float dot(const Vector2& other) const { return Vector2DotProduct(_vector, other._vector); }
    [[nodiscard]] float cross(const Vector2& other) const { return Vector2CrossProduct(_vector, other._vector); }
    [[nodiscard]] float distance(const Vector2& other) const { return Vector2Distance(_vector, other._vector); }
    [[nodiscard]] float angle(const Vector2& other) const { return Vector2Angle(_vector, other._vector); }
    [[nodiscard]] Vector2 normalized() const { return Vector2Normalize(_vector); }
    [[nodiscard]] Vector2 rotated(float angle) const { return Vector2Rotate(_vector, angle); }
    [[nodiscard]] Vector2 lerp(const Vector2& target, float amount) const {
        return Vector2Lerp(_vector, target._vector, amount);
    }
    [[nodiscard]] Vector2 movedTowards(const Vector2& target, float maxDistance) const {
        return Vector2MoveTowards(_vector, target._vector, maxDistance);
    }

    [[nodiscard]] Vector2 operator+(const Vector2& other) const { return Vector2Add(_vector, other._vector); }
    [[nodiscard]] Vector2 operator-(const Vector2& other) const { return Vector2Subtract(_vector, other._vector); }
    [[nodiscard]] Vector2 operator*(float scalar) const { return Vector2Scale(_vector, scalar); }
    [[nodiscard]] Vector2 operator/(float scalar) const {
        if (scalar != 0.0F) {
            return Vector2Scale(_vector, 1.0F / scalar);
        }
        return Vector2{0.0F, 0.0F};
    }
    [[nodiscard]] Vector2 operator-() const { return Vector2Negate(_vector); }
    [[nodiscard]] bool operator==(const Vector2& other) const { return Vector2Equals(_vector, other._vector) != 0; }

    Vector2& operator+=(const Vector2& other) {
        _vector = Vector2Add(_vector, other._vector);
        return *this;
    }
    Vector2& operator-=(const Vector2& other) {
        _vector = Vector2Subtract(_vector, other._vector);
        return *this;
    }
    Vector2& operator*=(float scalar) {
        _vector = Vector2Scale(_vector, scalar);
        return *this;
    }
    Vector2& operator/=(float scalar) {
        if (scalar != 0.0F) {
            _vector = Vector2Scale(_vector, 1.0F / scalar);
        }
        return *this;
    }

  protected:
  private:
    ::Vector2 _vector{};
};
}  // namespace zappy::gui::raylib::rmath
