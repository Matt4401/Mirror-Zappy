/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Rlgl
*/

#pragma once
#include <rlgl.h>

namespace zappy::gui::raylib::rcore {
class Rlgl {
  public:
    static void disableBackfaceCulling() { rlDisableBackfaceCulling(); }
    static void disableDepthMask() { rlDisableDepthMask(); }
    static void enableBackfaceCulling() { rlEnableBackfaceCulling(); }
    static void enableDepthMask() { rlEnableDepthMask(); }
};
}  // namespace zappy::gui::raylib::rcore
