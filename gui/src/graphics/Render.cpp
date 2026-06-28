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
#include <optional>

#include "AssetManager.hpp"
#include "AudioManager.hpp"
#include "FirstPerson.hpp"
#include "GuiEvents.hpp"
#include "SettingsManager.hpp"
#include "events/EventDispatcher.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector2.hpp"
#include "rmath/Vector3.hpp"
#include "scene/Tile3D.hpp"
#include "scene/WorldManager.hpp"
#include "ui/hud/GameHUD.hpp"

namespace zappy::gui::graphics {
Render::Render(events::EventDispatcher& dispatcher, AudioManager& audioManager, SettingsManager& settingsManager)
    : _skybox(dispatcher),
      _dispatcher(dispatcher),
      _audioManager(audioManager),
      _settingsManager(settingsManager),
      _worldManager(dispatcher, audioManager),
      _map(_camera, _worldManager, dispatcher, settingsManager) {
    _window.setTargetFPS(settingsManager.getSettings().fpsLimit);
    raylib::rcore::Window::setExitKey(0);

    AssetManager::getInstance().loadFont(DefaultFontName, "assets/fonts/Minecraft.ttf");

    _gameHUD = std::make_shared<ui::hud::GameHUD>(_dispatcher.get(), _audioManager.get(), _settingsManager.get(),
                                                  AssetManager::getInstance().getFont(DefaultFontName), _camera);
    _gameHUD->registerToUIManager(_uiManager);

    _firstPerson = std::make_unique<FirstPerson>(_dispatcher.get(), _camera, _worldManager, [this]() {
        if (_firstPerson && _firstPerson->active()) {
            _uiMode = false;
            _updateMode = UpdateMode::All;
        }
        updateCursorState();
    });

    if (auto pauseMenu = _gameHUD->getPauseMenu()) {
        pauseMenu->setOnResume([this]() {
            _updateMode = UpdateMode::All;
            updateCursorState();
        });
        pauseMenu->setOnExit([this]() { _isExiting = true; });
        pauseMenu->setOnResume([this, pauseMenu]() {
            pauseMenu->setVisible(false);
            _updateMode = UpdateMode::All;
            updateCursorState();
        });
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

    if (auto gameOverUI = _gameHUD->getGameOverUI()) {
        gameOverUI->setOnExit([this]() { _isExiting = true; });
        gameOverUI->setOnSpectate([this]() { updateCursorState(); });
    }

    _playerClickedToken = _dispatcher.get().subscribe<events::PlayerClicked>([this](const events::PlayerClicked& e) {
        _followedPlayerId = e.playerId;
        _lastPlayerPos = std::nullopt;
    });

    _playerUnselectedToken =
        _dispatcher.get().subscribe<events::PlayerUnselected>([this](const events::PlayerUnselected& /*e*/) {
            _followedPlayerId = -1;
            _lastPlayerPos = std::nullopt;
        });
}

Render::~Render() {
    if (_playerClickedToken != 0) {
        _dispatcher.get().unsubscribe<events::PlayerClicked>(_playerClickedToken);
    }
    if (_playerUnselectedToken != 0) {
        _dispatcher.get().unsubscribe<events::PlayerUnselected>(_playerUnselectedToken);
    }
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

void Render::updateFreeCamera() {
    auto& settings = _settingsManager.get().getSettings();
    auto& binds = settings.keybinds;

    float forward = 0.0F;
    float right = 0.0F;
    float up = 0.0F;

    auto isActionDown = [](int key) {
        if (key >= MouseButtonOffset && key <= MaxMouseButtonOffset) {
            return IsMouseButtonDown(key - MouseButtonOffset);
        }
        return raylib::rcore::Event::isKeyDown(key);
    };

    if (isActionDown(binds.at("Move Forward"))) {
        forward += 1.0F;
    }
    if (isActionDown(binds.at("Move Backward"))) {
        forward -= 1.0F;
    }
    if (isActionDown(binds.at("Move Right"))) {
        right += 1.0F;
    }
    if (isActionDown(binds.at("Move Left"))) {
        right -= 1.0F;
    }
    if (isActionDown(binds.at("Move Up"))) {
        up += 1.0F;
    }
    if (isActionDown(binds.at("Move Down"))) {
        up -= 1.0F;
    }

    float speed = CameraSpeed * raylib::rcore::Window::frameTime();
    if (isActionDown(binds.at("Sprint"))) {
        speed = CameraSprintSpeed * raylib::rcore::Window::frameTime();
    }

    raylib::rmath::Vector2 const mouseDelta = raylib::rcore::Event::getMouseDeltaStatic();
    float const sens = settings.cameraSensitivity * CameraSensMultiplier;

    raylib::rmath::Vector3 const movement(forward * speed, right * speed, up * speed);
    raylib::rmath::Vector3 const rotation(mouseDelta.x() * sens, mouseDelta.y() * sens, 0.0F);

    float const zoom = raylib::rcore::Event::getMouseWheelMoveStatic() * CameraZoomSpeed;

    _camera.updateCameraPro(movement, rotation, zoom);

    if (_camera.position().y() < scene::Tile3D::TILE_SIZE * MinCameraHeight) {
        _camera.setPosition(
            {_camera.position().x(), scene::Tile3D::TILE_SIZE * MinCameraHeight, _camera.position().z()});
        _camera.setTarget({_camera.target().x(), _camera.target().y() + (raylib::rcore::Window::frameTime() * 2.0F),
                           _camera.target().z()});
    }

    updateCameraLimits();
}

void Render::updateFollowCamera() {
    auto& settings = _settingsManager.get().getSettings();

    raylib::rmath::Vector2 const mouseDelta = raylib::rcore::Event::getMouseDeltaStatic();
    float const sens = settings.cameraSensitivity * CameraSensMultiplier;

    raylib::rmath::Vector3 const movement(0.0F, 0.0F, 0.0F);
    raylib::rmath::Vector3 const rotation(mouseDelta.x() * sens, mouseDelta.y() * sens, 0.0F);
    float const zoom = raylib::rcore::Event::getMouseWheelMoveStatic() * CameraZoomSpeed;

    _camera.updateCameraPro(movement, rotation, zoom);

    if (_camera.position().y() < scene::Tile3D::TILE_SIZE * MinCameraHeight) {
        _camera.setPosition(
            {_camera.position().x(), scene::Tile3D::TILE_SIZE * MinCameraHeight, _camera.position().z()});
        _camera.setTarget({_camera.target().x(), _camera.target().y() + (raylib::rcore::Window::frameTime() * 2.0F),
                           _camera.target().z()});
    }

    updateCameraLimits();
}

void Render::updateCursorState() const {
    if ((_firstPerson && _firstPerson->active()) ||
        (_gameHUD && _gameHUD->getPauseMenu() && _gameHUD->getPauseMenu()->isVisible()) ||
        (_gameHUD && _gameHUD->getGameOverUI() && _gameHUD->getGameOverUI()->isVisible()) || _uiMode) {
        raylib::rcore::Window::enableCursor();
    } else {
        raylib::rcore::Window::disableCursor();
    }
}

void Render::handleInput() {
    auto& binds = _settingsManager.get().getSettings().keybinds;
    auto isActionPressed = [](int key) {
        if (key >= MouseButtonOffset && key <= MaxMouseButtonOffset) {
            return raylib::rcore::Event::isMouseButtonPressed(key - MouseButtonOffset);
        }
        return raylib::rcore::Event::isKeyPressed(key);
    };

    if (isActionPressed(binds.at("Pause"))) {
        if (!_gameHUD || !_gameHUD->getPauseMenu() || !_gameHUD->getPauseMenu()->isSettingsVisible()) {
            handleEscapeKey();
        }
    }
    if (isActionPressed(binds.at("Toggle UI"))) {
        handleAltKey();
    }
}

void Render::handleEscapeKey() {
    if (_firstPerson && _firstPerson->active()) {
        _firstPerson->exit();
    } else if (_gameHUD && ((_gameHUD->getGridManager() && _gameHUD->getGridManager()->isConfigMode()) ||
                            (_gameHUD->getWorldControl() && _gameHUD->getWorldControl()->isConfigMode()))) {
        if (auto grid = _gameHUD->getGridManager()) {
            grid->setConfigMode(false);
            _gameHUD->saveUILayout();
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

void Render::updateCameraLimits() {
    const float mapWidth = static_cast<float>(_worldManager.width()) * scene::Tile3D::TILE_SIZE;
    const float mapDepth = static_cast<float>(_worldManager.height()) * scene::Tile3D::TILE_SIZE;
    const float maxMapDim = std::max(mapWidth, mapDepth);

    const float padding = std::clamp(maxMapDim * CameraPaddingFactor, CameraPaddingMin, CameraPaddingMax);
    const float limitX = (mapWidth * 0.5F) + padding;
    const float limitZ = (mapDepth * 0.5F) + padding;
    const float limitY = std::clamp(maxMapDim * CameraHeightFactor, CameraHeightMin, CameraHeightMax);

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

void Render::updateCameraState() {
    if (_followedPlayerId != -1) {
        auto playerOpt = _worldManager.playerById(_followedPlayerId);
        if (playerOpt) {
            auto const pos = playerOpt->get().position();
            raylib::rmath::Vector3 const delta =
                _lastPlayerPos.has_value() ? pos - *_lastPlayerPos : pos - _camera.target();
            _camera.setPosition(_camera.position() + delta);
            _camera.setTarget(pos);
            _lastPlayerPos = pos;

            if (!_firstPerson || !_firstPerson->active()) {
                updateFollowCamera();
            }
        } else {
            _followedPlayerId = -1;
            _lastPlayerPos = std::nullopt;
        }
    } else if (!_uiMode && (!_firstPerson || !_firstPerson->active())) {
        updateFreeCamera();
    }
}

void Render::updateHUDVisibility() const {
    if (_gameHUD) {
        bool const showUI = _settingsManager.get().getSettings().showUI;
        if (_gameHUD->getGridManager()) {
            _gameHUD->getGridManager()->setVisible(showUI);
        }
        if (_gameHUD->getCompass()) {
            _gameHUD->getCompass()->setVisible(showUI);
        }
    }
}

void Render::updateInteraction() {
    if (_uiMode && (!_firstPerson || !_firstPerson->active())) {
        if (!_uiManager.isHovered()) {
            _map.handleEvent();
        } else {
            _map.clearHoveredTile();
        }
    } else {
        _map.clearHoveredTile();
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

    updateCameraState();
    _camera.setFovy(_settingsManager.get().getSettings().fov);

    updateHUDVisibility();

    _audioManager.get().setListener(_camera.position(), _camera.target());

    updateInteraction();

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
