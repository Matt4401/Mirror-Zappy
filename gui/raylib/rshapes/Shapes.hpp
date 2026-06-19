/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Shapes
*/

#pragma once

#include <raylib.h>

#include "Color.hpp"

namespace zappy::gui::raylib::rshapes {

class Shapes {
  public:
    static void drawRectangleRec(Rectangle rec, Color color) { ::DrawRectangleRec(rec, color.color()); }

    static void drawRectangleRounded(Rectangle rec, float roundness, int segments, Color color) {
        ::DrawRectangleRounded(rec, roundness, segments, color.color());
    }

    static void drawRectangleLinesEx(Rectangle rec, float lineThick, Color color) {
        ::DrawRectangleLinesEx(rec, lineThick, color.color());
    }
};

}  // namespace zappy::gui::raylib::rshapes
