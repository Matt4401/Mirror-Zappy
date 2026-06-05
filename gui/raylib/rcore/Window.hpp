/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Window
*/

#pragma once
#include <raylib.h>

#include <cstdint>

namespace zappy::gui::raylib::rcore {
class Window {
  public:
    Window(int width = 800, int height = 600, const char* title = "Zappy", uint32_t flags = 0) {
        if (!IsWindowReady()) {
            initWindow(width, height, title, flags);
            _ownsWindow = true;
        }
    }
    ~Window() {
        if (_ownsWindow && IsWindowReady()) {
            closeWindow();
        }
    }

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;
    Window(Window&& other) = delete;
    Window& operator=(Window&& other) = delete;

    static void initWindow(int width = 800, int height = 600, const char* title = "Zappy", uint32_t flags = 0) {
        if (flags != 0) {
            SetConfigFlags(flags);
        }
        InitWindow(width, height, title);
    }
    static void closeWindow() { CloseWindow(); }
    static bool shouldClose() { return WindowShouldClose(); }

    static void beginDrawing() { BeginDrawing(); }
    static void endDrawing() { EndDrawing(); }
    static void clearBackground(Color color) { ClearBackground(color); }

    static void setTargetFPS(int fps) { SetTargetFPS(fps); }

    static double getScreenWidth() { return GetScreenWidth(); }
    static double getScreenHeight() { return GetScreenHeight(); }

  protected:
  private:
    bool _ownsWindow = false;
};
}  // namespace zappy::gui::raylib::rcore
