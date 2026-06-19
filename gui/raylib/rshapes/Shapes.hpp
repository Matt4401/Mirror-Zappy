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
        ::DrawRectangleRec({.x = rec.x, .y = rec.y, .width = rec.width, .height = rec.height}, color.color());
    }

    static void drawRectangleRounded(rmath::Rectangle rec, float roundness, int segments, Color color) {
        ::DrawRectangleRounded({.x = rec.x, .y = rec.y, .width = rec.width, .height = rec.height}, roundness, segments,
                               color.color());
    }

    static void drawRectangleLinesEx(rmath::Rectangle rec, float lineThick, Color color) {
        ::DrawRectangleLinesEx({.x = rec.x, .y = rec.y, .width = rec.width, .height = rec.height}, lineThick,
                               color.color());
    }

    static void drawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color) {
        ::DrawLine(startPosX, startPosY, endPosX, endPosY, color.color());
    }

    static bool checkCollisionPointRec(rmath::Vector2 point, rmath::Rectangle rec) {
        return ::CheckCollisionPointRec(point.vector(),
                                        {.x = rec.x, .y = rec.y, .width = rec.width, .height = rec.height});
    }
};

}  // namespace zappy::gui::raylib::rshapes
