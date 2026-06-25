/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#include "Render.hpp"

#include <raylib.h>

#include <memory>

#include "AssetManager.hpp"
#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "game/Player.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "scene/Tile3D.hpp"
#include "scene/WorldManager.hpp"
#include "ui/components/UIButton.hpp"
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

    _exitFirstPersonButton =
        std::make_shared<ui::components::UIButton>(0.0F, ExitViewButtonTop, ExitViewButtonWidth, ExitViewButtonHeight,
                                                   "Exit view", AssetManager::getInstance().getFont(DefaultFontName));
    _exitFirstPersonButton->setFontSize(ExitViewButtonFontSize);
    _exitFirstPersonButton->setVisible(false);
    _exitFirstPersonButton->setOnClick([this]() { exitFirstPersonView(); });
    _uiManager.addComponent(_exitFirstPersonButton);
    updateExitFirstPersonButtonPosition();

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

    _firstPersonToken = _dispatcher.get().subscribe<events::PlayerFirstPersonRequested>(
        [this](const events::PlayerFirstPersonRequested& event) { handleFirstPersonRequest(event); });
}

Render::~Render() {
    if (_firstPersonToken != 0) {
        _dispatcher.get().unsubscribe<events::PlayerFirstPersonRequested>(_firstPersonToken);
    }
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
    if (_firstPersonPlayerId.has_value() ||
        (_gameHUD && _gameHUD->getPauseMenu() && _gameHUD->getPauseMenu()->isVisible()) || _uiMode) {
        raylib::rcore::Window::enableCursor();
    } else {
        raylib::rcore::Window::disableCursor();
    }
}

void Render::handleInput() {
    if (raylib::rcore::Event::isKeyPressed(EscapeKey)) {
        if (_firstPersonPlayerId.has_value()) {
            exitFirstPersonView();
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

    if (raylib::rcore::Event::isKeyPressed(LeftAltKey)) {
        if (!_firstPersonPlayerId.has_value() &&
            (!_gameHUD || !_gameHUD->getGridManager() || !_gameHUD->getGridManager()->isConfigMode())) {
            _uiMode = !_uiMode;
            updateCursorState();
        }
    }
}

void Render::handleFirstPersonRequest(const events::PlayerFirstPersonRequested& event) {
    if (!_worldManager.playerById(event.playerId).has_value()) {
        return;
    }

    _firstPersonPlayerId = event.playerId;
    _uiMode = false;
    _updateMode = UpdateMode::All;
    if (_exitFirstPersonButton) {
        _exitFirstPersonButton->setVisible(true);
    }
    updateFirstPersonCamera();
    updateCursorState();
}

void Render::updateFirstPersonCamera() {
    if (!_firstPersonPlayerId.has_value()) {
        return;
    }
    const auto player = _worldManager.playerById(*_firstPersonPlayerId);
    if (!player.has_value()) {
        exitFirstPersonView();
        return;
    }

    const auto direction = player->get().renderDirection();
    const auto eyePosition = player->get().position() + raylib::rmath::Vector3{0.0F, FirstPersonEyeHeight, 0.0F};

    _camera.setPosition(eyePosition);
    _camera.setTarget(eyePosition + (direction * FirstPersonLookDistance));
    _camera.setUp({0.0F, 1.0F, 0.0F});
    _camera.setFovy(FirstPersonFovY);
    _camera.setProjection(PerspectiveProjection);
}

void Render::exitFirstPersonView() {
    _firstPersonPlayerId.reset();
    if (_exitFirstPersonButton) {
        _exitFirstPersonButton->setVisible(false);
    }
    updateCursorState();
}

void Render::updateExitFirstPersonButtonPosition() const {
    if (!_exitFirstPersonButton) {
        return;
    }
    const float buttonX = (static_cast<float>(raylib::rcore::Window::screenWidth()) - ExitViewButtonWidth) / 2.0F;
    _exitFirstPersonButton->setPosition(buttonX, ExitViewButtonTop);
}

void Render::update() {
    _event.update();
    handleInput();
    _worldManager.movePlayers(raylib::rcore::Window::frameTime());
    updateExitFirstPersonButtonPosition();
    updateFirstPersonCamera();

    _uiManager.update();
    _uiManager.handleEvent();

    if (_updateMode == UpdateMode::PauseMenuOnly) {
        return;
    }

    if (!_uiMode && !_firstPersonPlayerId.has_value()) {
        _camera.updateCamera(CAMERA_FREE);
        if (_camera.position().y() < scene::Tile3D::TILE_SIZE * 1.3F) {
            _camera.setPosition({_camera.position().x(), scene::Tile3D::TILE_SIZE * 1.3F, _camera.position().z()});
            _camera.setTarget({_camera.target().x(), _camera.target().y() + (raylib::rcore::Window::frameTime() * 2.0F),
                               _camera.target().z()});
        }
    }

    if (_uiMode && !_firstPersonPlayerId.has_value()) {
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
