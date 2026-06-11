/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Window
*/

#include "Window.hpp"

#include <raylib.h>

#include <cstdint>

namespace zappy::gui::raylib::rcore {
Window::Window(const char* title, uint32_t flags) {
    if (!IsWindowReady()) {
        if (flags != 0) {
            SetConfigFlags(flags);
        }
        _ownsWindow = true;
        _monitor = GetCurrentMonitor();
        _width = GetMonitorWidth(_monitor);
        _height = GetMonitorHeight(_monitor);
        InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, title);
        DisableCursor();
        ToggleFullscreen();
    }
}

Window::~Window() {
    if (_ownsWindow && IsWindowReady()) {
        CloseWindow();
    }
}

void Window::setTargetFPS(int fps) {
    _fps = fps;
    SetTargetFPS(_fps);
}

int Window::getScreenWidth() {
    _width = GetScreenWidth();
    return _width;
}

int Window::getScreenHeight() {
    _height = GetScreenHeight();
    return _height;
}
}  // namespace zappy::gui::raylib::rcore
