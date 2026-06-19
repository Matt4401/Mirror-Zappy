/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Shapes
*/

#pragma once

#include <raylib.h>

#include "Color.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"

namespace zappy::gui::raylib::rshapes {

class Shapes {
  public:
    static void drawRectangleRec(rmath::Rectangle rec, Color color) {
        ::DrawRectangleRec({rec.x, rec.y, rec.width, rec.height}, color.color());
    }

    static void drawRectangleRounded(rmath::Rectangle rec, float roundness, int segments, Color color) {
        ::DrawRectangleRounded({rec.x, rec.y, rec.width, rec.height}, roundness, segments, color.color());
    }

    static void drawRectangleLinesEx(rmath::Rectangle rec, float lineThick, Color color) {
        ::DrawRectangleLinesEx({rec.x, rec.y, rec.width, rec.height}, lineThick, color.color());
    }

    static void drawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color) {
        ::DrawLine(startPosX, startPosY, endPosX, endPosY, color.color());
    }

    static bool checkCollisionPointRec(rmath::Vector2 point, rmath::Rectangle rec) {
        return ::CheckCollisionPointRec(point.vector(), {rec.x, rec.y, rec.width, rec.height});
    }
};

}  // namespace zappy::gui::raylib::rshapes
