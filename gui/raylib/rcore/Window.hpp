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
    static constexpr int DEFAULT_WIDTH = 600;
    static constexpr int DEFAULT_HEIGHT = 400;

    Window(const char* title, uint32_t flags = 0) {
        if (!IsWindowReady()) {
            if (flags != 0) {
                SetConfigFlags(flags);
            }
            _ownsWindow = true;
            _monitor = GetCurrentMonitor();
            _width = GetMonitorWidth(_monitor);
            _height = GetMonitorHeight(_monitor);
            InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, title);
            ToggleFullscreen();
            DisableCursor();
        }
    }
    ~Window() {
        if (_ownsWindow && IsWindowReady()) {
            CloseWindow();
        }
    }

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;
    Window(Window&& other) = delete;
    Window& operator=(Window&& other) = delete;

    [[nodiscard]] bool shouldClose() const { return _ownsWindow && WindowShouldClose(); }

    static void beginDrawing() { BeginDrawing(); }
    static void endDrawing() { EndDrawing(); }
    void clearBackground() { ClearBackground(_backgroundColor); }

    void setTargetFPS(int fps) {
        _fps = fps;
        SetTargetFPS(_fps);
    }

    int getScreenWidth() {
        _width = GetScreenWidth();
        return _width;
    }

    int getScreenHeight() {
        _height = GetScreenHeight();
        return _height;
    }

  protected:
  private:
    bool _ownsWindow = false;
    int _fps{60};
    Color _backgroundColor{WHITE};
    int _width{0};
    int _height{0};
    int _monitor{0};
};
}  // namespace zappy::gui::raylib::rcore
