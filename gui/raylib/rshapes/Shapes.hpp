/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Shapes
*/

#pragma once

#include <raylib.h>
#include <rlgl.h>

#include <array>

#include "Color.hpp"
#include "rcore/BoundingBox.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"
#include "rmath/Vector3.hpp"

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

    static void drawLineEx(rmath::Vector2 startPos, rmath::Vector2 endPos, float thick, Color color) {
        ::DrawLineEx(startPos.vector(), endPos.vector(), thick, color.color());
    }

    static void drawTriangle(rmath::Vector2 v1, rmath::Vector2 v2, rmath::Vector2 v3, Color color) {
        ::DrawTriangle(v1.vector(), v2.vector(), v3.vector(), color.color());
    }

    static bool checkCollisionPointRec(rmath::Vector2 point, rmath::Rectangle rec) {
        return ::CheckCollisionPointRec(point.vector(),
                                        {.x = rec.x, .y = rec.y, .width = rec.width, .height = rec.height});
    }
    static void drawBoundingBox(rcore::BoundingBox box, Color color, float thickness = 1.0F) {
        ::rlSetLineWidth(thickness);
        ::DrawBoundingBox(box.boundingBox(), color.color());
        ::rlSetLineWidth(1.0F);
    }

    static void drawCube(rmath::Vector3 position, float width, float height, float length, Color color) {
        ::DrawCube(position.vector(), width, height, length, color.color());
    }

    static void drawThickBoundingBox(rcore::BoundingBox box, Color color, float thick) {
        ::BoundingBox const bb = box.boundingBox();
        float const w = bb.max.x - bb.min.x;
        float const h = bb.max.y - bb.min.y;
        float const d = bb.max.z - bb.min.z;

        std::array<float, 2> const x = {bb.min.x, bb.max.x};
        std::array<float, 2> const y = {bb.min.y, bb.max.y};
        std::array<float, 2> const z = {bb.min.z, bb.max.z};

        drawCube({x.at(0) + (w / 2.0F), y.at(0), z.at(0)}, w, thick, thick, color);
        drawCube({x.at(0) + (w / 2.0F), y.at(0), z.at(1)}, w, thick, thick, color);
        drawCube({x.at(0), y.at(0), z.at(0) + (d / 2.0F)}, thick, thick, d, color);
        drawCube({x.at(1), y.at(0), z.at(0) + (d / 2.0F)}, thick, thick, d, color);

        drawCube({x.at(0) + (w / 2.0F), y.at(1), z.at(0)}, w, thick, thick, color);
        drawCube({x.at(0) + (w / 2.0F), y.at(1), z.at(1)}, w, thick, thick, color);
        drawCube({x.at(0), y.at(1), z.at(0) + (d / 2.0F)}, thick, thick, d, color);
        drawCube({x.at(1), y.at(1), z.at(0) + (d / 2.0F)}, thick, thick, d, color);

        drawCube({x.at(0), y.at(0) + (h / 2.0F), z.at(0)}, thick, h, thick, color);
        drawCube({x.at(0), y.at(0) + (h / 2.0F), z.at(1)}, thick, h, thick, color);
        drawCube({x.at(1), y.at(0) + (h / 2.0F), z.at(0)}, thick, h, thick, color);
        drawCube({x.at(1), y.at(0) + (h / 2.0F), z.at(1)}, thick, h, thick, color);
    }
};

}  // namespace zappy::gui::raylib::rshapes
