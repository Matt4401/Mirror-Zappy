/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#include "Render.hpp"

#include <raylib.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "AssetManager.hpp"
#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "protocol/Commands.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "scene/Tile3D.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIGamePanel.hpp"
#include "ui/components/UIGridManager.hpp"
#include "ui/components/UIText.hpp"
#include "ui/menus/PauseMenu.hpp"

namespace zappy::gui::graphics {
Render::Render(std::shared_ptr<events::EventDispatcher> dispatcher)
    : _dispatcher(std::move(dispatcher)), _map(2, 2, _camera, _dispatcher) {
    if (_dispatcher) {
        _mszToken = _dispatcher->subscribe<shared::protocol::server::Msz>(
            [this](const shared::protocol::server::Msz& cmd) { _map.resize(cmd.width, cmd.height); });
        _sgtToken = _dispatcher->subscribe<shared::protocol::server::Sgt>(
            [this](const shared::protocol::server::Sgt& cmd) { _serverFrequency = static_cast<float>(cmd.timeUnit); });
        _playerClickedToken = _dispatcher->subscribe<events::PlayerClicked>([this](const events::PlayerClicked& event) {
            std::cout << "Player clicked: " << event.playerName << " from team " << event.teamName << " at position ("
                      << event.position.x() << ", " << event.position.y() << ", " << event.position.z() << ")\n";
        });
    }
    _window.setTargetFPS(DefaultFps);
    raylib::rcore::Window::setExitKey(0);

    AssetManager::getInstance().loadFont(DefaultFontName, "assets/fonts/Minecraft.ttf");

    // Temporary tests panels
    _gridManager = std::make_shared<ui::components::UIGridManager>();
    _uiManager.addComponent(_gridManager);

    _demoPanel = std::make_shared<ui::components::UIGamePanel>(0, 0, 0, 0, "Player Info");
    auto font = AssetManager::getInstance().getFont(DefaultFontName);
    auto btn = std::make_shared<ui::components::UIButton>(100.0F, 120.0F, 200.0F, 40.0F, "Click me!", font);
    _demoPanel->addComponent(btn);

    auto _demoPanel2 = std::make_shared<ui::components::UIGamePanel>(0, 0, 0, 0, "Inventory");
    auto text2 = std::make_shared<ui::components::UIText>("Empty", font);
    text2->setPosition(40.0F, 200.0F);
    _demoPanel2->addComponent(text2);

    _gridManager->addPanel(_demoPanel, 1, 1, 12, 18);
    _gridManager->addPanel(_demoPanel2, 1, 19, 12, 8);

    _pauseMenu =
        std::make_shared<ui::menus::PauseMenu>(_dispatcher, AssetManager::getInstance().getFont(DefaultFontName));
    _pauseMenu->setOnExit([this]() { _isExiting = true; });
    _pauseMenu->setOnUIConfig([this]() {
        _gridManager->setConfigMode(true);
        _uiMode = true;
        _pauseMenu->setVisible(false);
    });
    _uiManager.addComponent(_pauseMenu);
}

Render::~Render() {
    if (_dispatcher) {
        if (_mszToken != 0) {
            _dispatcher->unsubscribe<shared::protocol::server::Msz>(_mszToken);
        }
        if (_sgtToken != 0) {
            _dispatcher->unsubscribe<shared::protocol::server::Sgt>(_sgtToken);
        }
    }
    if (_dispatcher && _playerClickedToken != 0) {
        _dispatcher->unsubscribe<events::PlayerClicked>(_playerClickedToken);
    }
    _pauseMenu.reset();
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
    if (_pauseMenu->isVisible() || _uiMode) {
        raylib::rcore::Window::enableCursor();
    } else {
        raylib::rcore::Window::disableCursor();
    }
}

void Render::handleInput() {
    if (raylib::rcore::Event::isKeyPressed(EscapeKey)) {
        if (_gridManager && _gridManager->isConfigMode()) {
            _gridManager->setConfigMode(false);
            _uiMode = false;
            _updateMode = UpdateMode::All;
        } else {
            bool const pauseVisible = !_pauseMenu->isVisible();
            _pauseMenu->setVisible(pauseVisible);
            _updateMode = pauseVisible ? UpdateMode::PauseMenuOnly : UpdateMode::All;
        }
        updateCursorState();
    }

    if (raylib::rcore::Event::isKeyPressed(LeftAltKey)) {
        if (!_gridManager || !_gridManager->isConfigMode()) {
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

    _skybox.update(raylib::rcore::Window::frameTime(), _serverFrequency);
}

void Render::render2D() {}

void Render::render3D() {
    _camera->beginMode3D();
    _skybox.draw();
    _map.draw();
    raylib::rcore::Camera::endMode3D();
}
}  // namespace zappy::gui::graphics
