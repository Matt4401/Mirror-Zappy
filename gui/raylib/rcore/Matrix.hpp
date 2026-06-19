/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Matrix
*/

#pragma once
#include <raylib.h>
#include <raymath.h>

namespace zappy::gui::raylib::rcore {
class Matrix {
  public:
    Matrix(float m0, float m4, float m8, float m12, float m1, float m5, float m9, float m13, float m2, float m6,
           float m10, float m14, float m3, float m7, float m11, float m15)
        : _matrix(m0, m4, m8, m12, m1, m5, m9, m13, m2, m6, m10, m14, m3, m7, m11, m15) {};
    Matrix(::Matrix matrix) : _matrix(matrix) {};
    Matrix() : _matrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) {};
    ~Matrix() = default;
    Matrix(const Matrix& other) = default;
    Matrix& operator=(const Matrix& other) = default;
    Matrix(Matrix&& other) = default;
    Matrix& operator=(Matrix&& other) = default;

    [[nodiscard]] ::Matrix matrix() const { return _matrix; }

    static Matrix rotateY(float angle) { return Matrix(MatrixRotateY(angle)); }

  protected:
  private:
    ::Matrix _matrix{};
};
}  // namespace zappy::gui::raylib::rcore
