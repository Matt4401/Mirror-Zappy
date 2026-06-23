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
        : _matrix{.m0 = m0,
                  .m4 = m4,
                  .m8 = m8,
                  .m12 = m12,
                  .m1 = m1,
                  .m5 = m5,
                  .m9 = m9,
                  .m13 = m13,
                  .m2 = m2,
                  .m6 = m6,
                  .m10 = m10,
                  .m14 = m14,
                  .m3 = m3,
                  .m7 = m7,
                  .m11 = m11,
                  .m15 = m15} {};
    Matrix(::Matrix matrix) : _matrix(matrix) {};
    Matrix()
        : _matrix{.m0 = 0,
                  .m4 = 0,
                  .m8 = 0,
                  .m12 = 0,
                  .m1 = 0,
                  .m5 = 0,
                  .m9 = 0,
                  .m13 = 0,
                  .m2 = 0,
                  .m6 = 0,
                  .m10 = 0,
                  .m14 = 0,
                  .m3 = 0,
                  .m7 = 0,
                  .m11 = 0,
                  .m15 = 0} {};
    ~Matrix() = default;
    Matrix(const Matrix& other) = default;
    Matrix& operator=(const Matrix& other) = default;
    Matrix(Matrix&& other) = default;
    Matrix& operator=(Matrix&& other) = default;

    [[nodiscard]] ::Matrix matrix() const { return _matrix; }

    static Matrix rotateY(float angle) { return {MatrixRotateY(angle)}; }

  protected:
  private:
    ::Matrix _matrix{};
};
}  // namespace zappy::gui::raylib::rcore
