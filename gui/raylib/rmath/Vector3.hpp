/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Vector3
*/

#pragma once
#include <raylib.h>
#include <raymath.h>

namespace zappy::gui::raylib::rmath {
class Vector3 {
  public:
    constexpr Vector3() = default;
    constexpr Vector3(int x, int y, int z)
        : _vector{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)} {}
    constexpr Vector3(float x, float y, float z) : _vector{x, y, z} {}
    constexpr Vector3(::Vector3 vector) : _vector{vector} {}

    [[nodiscard]] constexpr float x() const { return _vector.x; }
    [[nodiscard]] constexpr float y() const { return _vector.y; }
    [[nodiscard]] constexpr float z() const { return _vector.z; }
    [[nodiscard]] constexpr ::Vector3 vector() const { return _vector; }
    [[nodiscard]] constexpr ::Vector3& vector() { return _vector; }

    constexpr operator ::Vector3() const { return _vector; }

    constexpr void setX(float x) { _vector.x = x; }
    constexpr void setY(float y) { _vector.y = y; }
    constexpr void setZ(float z) { _vector.z = z; }

    [[nodiscard]] float length() const { return Vector3Length(_vector); }
    [[nodiscard]] float lengthSquared() const { return Vector3LengthSqr(_vector); }
    [[nodiscard]] float dot(const Vector3& other) const { return Vector3DotProduct(_vector, other._vector); }
    [[nodiscard]] float distance(const Vector3& other) const { return Vector3Distance(_vector, other._vector); }
    [[nodiscard]] float angle(const Vector3& other) const { return Vector3Angle(_vector, other._vector); }
    [[nodiscard]] Vector3 cross(const Vector3& other) const { return Vector3CrossProduct(_vector, other._vector); }
    [[nodiscard]] Vector3 normalized() const { return Vector3Normalize(_vector); }
    [[nodiscard]] Vector3 lerp(const Vector3& target, float amount) const {
        return Vector3Lerp(_vector, target._vector, amount);
    }
    [[nodiscard]] Vector3 movedTowards(const Vector3& target, float maxDistance) const {
        return Vector3MoveTowards(_vector, target._vector, maxDistance);
    }

    [[nodiscard]] Vector3 operator+(const Vector3& other) const { return Vector3Add(_vector, other._vector); }
    [[nodiscard]] Vector3 operator-(const Vector3& other) const { return Vector3Subtract(_vector, other._vector); }
    [[nodiscard]] Vector3 operator*(float scalar) const { return Vector3Scale(_vector, scalar); }
    [[nodiscard]] Vector3 operator/(float scalar) const {
        if (scalar != 0.0F) {
            return Vector3Scale(_vector, 1.0F / scalar);
        }
        return Vector3{0.0F, 0.0F, 0.0F};
    }
    [[nodiscard]] Vector3 operator-() const { return Vector3Negate(_vector); }
    [[nodiscard]] bool operator==(const Vector3& other) const { return Vector3Equals(_vector, other._vector) != 0; }

    Vector3& operator+=(const Vector3& other) {
        _vector = Vector3Add(_vector, other._vector);
        return *this;
    }
    Vector3& operator-=(const Vector3& other) {
        _vector = Vector3Subtract(_vector, other._vector);
        return *this;
    }
    Vector3& operator*=(float scalar) {
        _vector = Vector3Scale(_vector, scalar);
        return *this;
    }
    Vector3& operator/=(float scalar) {
        if (scalar != 0.0F) {
            _vector = Vector3Scale(_vector, 1.0F / scalar);
        } else {
            _vector = ::Vector3{0.0F, 0.0F, 0.0F};
        }
        return *this;
    }

  protected:
  private:
    ::Vector3 _vector{};
};
}  // namespace zappy::gui::raylib::rmath
