/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Window
*/

#pragma once
#include <raylib.h>

#include <cstdint>

#include "Color.hpp"

namespace zappy::gui::raylib::rcore {
class Window {
  public:
    static constexpr int DEFAULT_WIDTH = 600;
    static constexpr int DEFAULT_HEIGHT = 400;

    Window(const char* title, uint32_t flags = 0);
    ~Window();

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;
    Window(Window&& other) = delete;
    Window& operator=(Window&& other) = delete;

    [[nodiscard]] bool shouldClose() const { return _ownsWindow && WindowShouldClose(); }

    void beginDrawing() {
        BeginDrawing();
        clearBackground();
    }
    static void endDrawing() { EndDrawing(); }
    static float frameTime() { return GetFrameTime(); }
    void clearBackground() { ClearBackground(_backgroundColor.color()); }
    static void clearBackground(const raylib::Color& color) { ClearBackground(color.color()); }

    static void beginScissorMode(int x, int y, int width, int height) { BeginScissorMode(x, y, width, height); }
    static void endScissorMode() { EndScissorMode(); }

    void setTargetFPS(int fps);
    int getScreenWidth();
    int getScreenHeight();

    static void setExitKey(int key) { SetExitKey(key); }
    static void enableCursor() { EnableCursor(); }
    static void disableCursor() { DisableCursor(); }
    static bool isCursorHidden() { return IsCursorHidden(); }
    static int screenWidth() { return GetScreenWidth(); }
    static int screenHeight() { return GetScreenHeight(); }

  protected:
  private:
    bool _ownsWindow = false;
    int _fps{60};
    Color _backgroundColor{Color::White()};
    int _width{0};
    int _height{0};
    int _monitor{0};
};
}  // namespace zappy::gui::raylib::rcore
