/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#include "Render.hpp"

#include <raylib.h>

#include <algorithm>
#include <memory>

#include "AssetManager.hpp"
#include "FirstPerson.hpp"
#include "events/EventDispatcher.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector3.hpp"
#include "scene/Tile3D.hpp"
#include "scene/WorldManager.hpp"
#include "ui/hud/GameHUD.hpp"

namespace zappy::gui::graphics {
Render::Render(events::EventDispatcher& dispatcher)
    : _skybox(dispatcher),
      _dispatcher(dispatcher),
      _worldManager(dispatcher),
      _map(_camera, _worldManager, dispatcher) {
    _window.setTargetFPS(DefaultFps);
    raylib::rcore::Window::setExitKey(0);

    AssetManager::getInstance().loadFont(DefaultFontName, "assets/fonts/Minecraft.ttf");

    _gameHUD = std::make_shared<ui::hud::GameHUD>(_dispatcher.get(),
                                                  AssetManager::getInstance().getFont(DefaultFontName), _camera);
    _gameHUD->registerToUIManager(_uiManager);

    _firstPerson = std::make_unique<FirstPerson>(_dispatcher.get(), _camera, _worldManager, [this]() {
        if (_firstPerson && _firstPerson->active()) {
            _uiMode = false;
            _updateMode = UpdateMode::All;
        }
        updateCursorState();
    });

    _keyHandlers = {
        {EscapeKey, [this]() { handleEscapeKey(); }},
        {LeftAltKey, [this]() { handleAltKey(); }},
    };

    if (auto pauseMenu = _gameHUD->getPauseMenu()) {
        pauseMenu->setOnExit([this]() { _isExiting = true; });
        pauseMenu->setOnUIConfig([this, pauseMenu]() {
            if (auto grid = _gameHUD->getGridManager()) {
                grid->setConfigMode(true);
            }
            if (auto worldControl = _gameHUD->getWorldControl()) {
                worldControl->setConfigMode(true);
            }
            _uiMode = true;
            pauseMenu->setVisible(false);
        });
    }
}

Render::~Render() {
    _firstPerson.reset();
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
    if ((_firstPerson && _firstPerson->active()) ||
        (_gameHUD && _gameHUD->getPauseMenu() && _gameHUD->getPauseMenu()->isVisible()) || _uiMode) {
        raylib::rcore::Window::enableCursor();
    } else {
        raylib::rcore::Window::disableCursor();
    }
}

void Render::handleInput() {
    for (const auto& [key, handler] : _keyHandlers) {
        if (raylib::rcore::Event::isKeyPressed(key)) {
            handler();
        }
    }
}

void Render::handleEscapeKey() {
    if (_firstPerson && _firstPerson->active()) {
        _firstPerson->exit();
    } else if (_gameHUD && ((_gameHUD->getGridManager() && _gameHUD->getGridManager()->isConfigMode()) ||
                            (_gameHUD->getWorldControl() && _gameHUD->getWorldControl()->isConfigMode()))) {
        if (auto grid = _gameHUD->getGridManager()) {
            grid->setConfigMode(false);
        }
        if (auto worldControl = _gameHUD->getWorldControl()) {
            worldControl->setConfigMode(false);
        }
        _uiMode = false;
        _updateMode = UpdateMode::All;
    } else if (_gameHUD && _gameHUD->getPauseMenu()) {
        bool const pauseVisible = !_gameHUD->getPauseMenu()->isVisible();
        _gameHUD->getPauseMenu()->setVisible(pauseVisible);
        _updateMode = pauseVisible ? UpdateMode::PauseMenuOnly : UpdateMode::All;
    }
    updateCursorState();
}

void Render::handleAltKey() {
    if ((!_firstPerson || !_firstPerson->active()) &&
        (!_gameHUD || !_gameHUD->getGridManager() || !_gameHUD->getGridManager()->isConfigMode())) {
        _uiMode = !_uiMode;
        updateCursorState();
    }
}

void Render::update() {
    _event.update();
    handleInput();
    _worldManager.movePlayers(raylib::rcore::Window::frameTime());
    if (_firstPerson) {
        _firstPerson->update();
    }

    _uiManager.update();
    _uiManager.handleEvent();

    if (_updateMode == UpdateMode::PauseMenuOnly) {
        return;
    }

    if (!_uiMode && (!_firstPerson || !_firstPerson->active())) {
        _camera.updateCamera(CAMERA_FREE);
        if (_camera.position().y() < scene::Tile3D::TILE_SIZE * MinCameraHeight) {
            _camera.setPosition(
                {_camera.position().x(), scene::Tile3D::TILE_SIZE * MinCameraHeight, _camera.position().z()});
            _camera.setTarget({_camera.target().x(), _camera.target().y() + (raylib::rcore::Window::frameTime() * 2.0F),
                               _camera.target().z()});
        }

        const float mapWidth = static_cast<float>(_worldManager.width()) * scene::Tile3D::TILE_SIZE;
        const float mapDepth = static_cast<float>(_worldManager.height()) * scene::Tile3D::TILE_SIZE;
        const float padding = (std::max(mapWidth, mapDepth) * 0.5F) + DepthLimit;

        const float limitX = (mapWidth * 0.5F) + padding;
        const float limitZ = (mapDepth * 0.5F) + padding;
        const float limitY = (std::max(mapWidth, mapDepth));

        raylib::rmath::Vector3 pos = _camera.position();
        bool clamped = false;

        if (pos.x() > limitX) {
            pos.setX(limitX);
            clamped = true;
        }
        if (pos.x() < -limitX) {
            pos.setX(-limitX);
            clamped = true;
        }

        if (pos.z() > limitZ) {
            pos.setZ(limitZ);
            clamped = true;
        }
        if (pos.z() < -limitZ) {
            pos.setZ(-limitZ);
            clamped = true;
        }

        if (pos.y() > limitY) {
            pos.setY(limitY);
            clamped = true;
        }

        if (clamped) {
            _camera.setPosition(pos);
        }
    }

    if (_uiMode && (!_firstPerson || !_firstPerson->active())) {
        if (!_uiManager.isHovered()) {
            _map.handleEvent();
        } else {
            _map.clearHoveredTile();
        }
    } else {
        _map.clearHoveredTile();
    }

    _skybox.update(raylib::rcore::Window::frameTime(), static_cast<float>(_worldManager.timeUnit()));
}

void Render::render2D() {}

void Render::render3D() {
    _camera.beginMode3D();
    _skybox.draw();
    _map.draw();
    raylib::rcore::Camera::endMode3D();
}
}  // namespace zappy::gui::graphics
