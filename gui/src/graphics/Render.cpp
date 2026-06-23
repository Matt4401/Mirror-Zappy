/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#include "Render.hpp"

#include <raylib.h>

#include <memory>
#include <string>
#include <utility>

#include "AssetManager.hpp"
#include "events/EventDispatcher.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "scene/Tile3D.hpp"
#include "scene/WorldManager.hpp"
#include "ui/hud/GameHUD.hpp"

namespace zappy::gui::graphics {
Render::Render(std::shared_ptr<events::EventDispatcher> dispatcher)
    : _dispatcher(std::move(dispatcher)), _worldManager(_dispatcher), _map(_camera, _worldManager, _dispatcher) {
    _window.setTargetFPS(DefaultFps);
    raylib::rcore::Window::setExitKey(0);

    AssetManager::getInstance().loadFont(DefaultFontName, "assets/fonts/Minecraft.ttf");

    _gameHUD = std::make_shared<ui::hud::GameHUD>(_dispatcher, AssetManager::getInstance().getFont(DefaultFontName));
    _gameHUD->registerToUIManager(_uiManager);

    if (auto pauseMenu = _gameHUD->getPauseMenu()) {
        pauseMenu->setOnExit([this]() { _isExiting = true; });
        pauseMenu->setOnUIConfig([this, pauseMenu]() {
            if (auto grid = _gameHUD->getGridManager()) {
                grid->setConfigMode(true);
            }
            _uiMode = true;
            pauseMenu->setVisible(false);
        });
    }
}

Render::~Render() {
    _gameHUD.reset();
    _uiManager.clear();
    AssetManager::getInstance().clear();
}

bool Render::isRunning() const { return !_window.shouldClose() && !_isExiting; }

void Render::renderFrame() {
    update();
    _window.beginDrawing();
    render2D();
    render3D();
    _uiManager.draw();
    raylib::rcore::Window::endDrawing();
}

void Render::updateCursorState() const {
    if ((_gameHUD && _gameHUD->getPauseMenu() && _gameHUD->getPauseMenu()->isVisible()) || _uiMode) {
        raylib::rcore::Window::enableCursor();
    } else {
        raylib::rcore::Window::disableCursor();
    }
}

void Render::handleInput() {
    if (raylib::rcore::Event::isKeyPressed(EscapeKey)) {
        if (_gameHUD && _gameHUD->getGridManager() && _gameHUD->getGridManager()->isConfigMode()) {
            _gameHUD->getGridManager()->setConfigMode(false);
            _uiMode = false;
            _updateMode = UpdateMode::All;
        } else if (_gameHUD && _gameHUD->getPauseMenu()) {
            bool const pauseVisible = !_gameHUD->getPauseMenu()->isVisible();
            _gameHUD->getPauseMenu()->setVisible(pauseVisible);
            _updateMode = pauseVisible ? UpdateMode::PauseMenuOnly : UpdateMode::All;
        }
        updateCursorState();
    }

    if (raylib::rcore::Event::isKeyPressed(LeftAltKey)) {
        if (!_gameHUD || !_gameHUD->getGridManager() || !_gameHUD->getGridManager()->isConfigMode()) {
            _uiMode = !_uiMode;
            updateCursorState();
        }
    }
}

void Render::update() {
    _event.update();
    handleInput();

    _uiManager.update();
    _uiManager.handleEvent(_event);

    if (_updateMode == UpdateMode::PauseMenuOnly) {
        return;
    }

    if (!_uiMode) {
        _camera->updateCamera(CAMERA_FREE);
        if (_camera->position().y() < scene::Tile3D::TILE_SIZE * 1.3F) {
            _camera->setPosition({_camera->position().x(), scene::Tile3D::TILE_SIZE * 1.3F, _camera->position().z()});
            _camera->setTarget({_camera->target().x(),
                                _camera->target().y() + (raylib::rcore::Window::frameTime() * 2.0F),
                                _camera->target().z()});
        }
    }

    if (_uiMode) {
        _map.handleEvent(_event);
    }

    _skybox.update(raylib::rcore::Window::frameTime(), static_cast<float>(_worldManager.timeUnit()));
}

void Render::render2D() {}

void Render::render3D() {
    _camera->beginMode3D();
    _skybox.draw();
    _map.draw();
    raylib::rcore::Camera::endMode3D();
}
}  // namespace zappy::gui::graphics
