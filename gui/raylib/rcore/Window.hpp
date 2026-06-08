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
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    Window(int width = WIDTH, int height = HEIGHT, const char* title = "Zappy", uint32_t flags = 0)
        : _width(width), _height(height) {
        if (!IsWindowReady()) {
            if (flags != 0) {
                SetConfigFlags(flags);
            }
            InitWindow(width, height, title);
            _ownsWindow = true;
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
    Color _backgroundColor{BLACK};
    int _width{WIDTH};
    int _height{HEIGHT};
};
}  // namespace zappy::gui::raylib::rcore
