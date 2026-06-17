/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Render
*/

#include "Render.hpp"

#include <raylib.h>

#include <memory>
#include <utility>

#include "AssetManager.hpp"
#include "events/EventDispatcher.hpp"
#include "protocol/Commands.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIGamePanel.hpp"
#include "ui/components/UIText.hpp"
#include "ui/menus/PauseMenu.hpp"

namespace zappy::gui::graphics {
Render::Render(std::shared_ptr<events::EventDispatcher> dispatcher) : _dispatcher(std::move(dispatcher)) {
    if (_dispatcher) {
        _mszToken = _dispatcher->subscribe<shared::protocol::server::Msz>(
            [this](const shared::protocol::server::Msz& cmd) { _map.resize(cmd.width, cmd.height); });
    }
    _window.setTargetFPS(60);
    raylib::rcore::Window::setExitKey(0);

    AssetManager::getInstance().loadFont("Minecraft", "assets/fonts/Minecraft.ttf");

    // Temporary tests panels
    _demoPanel = std::make_shared<ui::components::UIGamePanel>(50.0F, 50.0F, 300.0F, 400.0F, "Player Info");
    auto font = AssetManager::getInstance().getFont("Minecraft");
    auto btn = std::make_shared<ui::components::UIButton>(100.0F, 120.0F, 200.0F, 40.0F, "Click me!", font);
    auto text = std::make_shared<ui::components::UIText>("Level 8 Player", font);
    text->setPosition(100.0F, 200.0F);
    _demoPanel->addComponent(btn);
    _demoPanel->addComponent(text);
    _uiManager.addComponent(_demoPanel);

    auto _demoPanel2 = std::make_shared<ui::components::UIGamePanel>(50.0F, 460.0F, 300.0F, 200.0F, "Inventory");
    auto text2 = std::make_shared<ui::components::UIText>("Empty", font);
    text2->setPosition(100.0F, 520.0F);
    _demoPanel2->addComponent(text2);
    _uiManager.addComponent(_demoPanel2);
    _demoPanel->setNextPanel(_demoPanel2);

    _pauseMenu = std::make_shared<ui::menus::PauseMenu>(_dispatcher, AssetManager::getInstance().getFont("Minecraft"));
    _pauseMenu->setOnExit([this]() { _isExiting = true; });
    _uiManager.addComponent(_pauseMenu);
}

Render::~Render() {
    if (_dispatcher && _mszToken != 0) {
        _dispatcher->unsubscribe<shared::protocol::server::Msz>(_mszToken);
    }
    _pauseMenu.reset();
    _uiManager.clear();
    AssetManager::getInstance().clear();
}

bool Render::isRunning() const { return !_window.shouldClose() && !_isExiting; }

void Render::renderFrame() {
    update();
    if (!_pauseMenu->isVisible()) {
        _event.handleEvent(_eventContext);
    }
    _window.beginDrawing();
    render2D();
    render3D();
    _uiManager.draw();
    raylib::rcore::Window::endDrawing();
}

void Render::update() {
    if (raylib::rcore::Event::isKeyPressed(EscapeKey)) {
        _pauseMenu->setVisible(!_pauseMenu->isVisible());
        if (_pauseMenu->isVisible() || _uiMode) {
            raylib::rcore::Window::enableCursor();
        } else {
            raylib::rcore::Window::disableCursor();
        }
    }

    if (raylib::rcore::Event::isKeyPressed(LeftAltKey)) {
        _uiMode = !_uiMode;
        if (_pauseMenu->isVisible() || _uiMode) {
            raylib::rcore::Window::enableCursor();
        } else {
            raylib::rcore::Window::disableCursor();
        }
    }

    _uiManager.update();
    _uiManager.handleEvent(_event);

    if (!_pauseMenu->isVisible() && !_uiMode) {
        _camera->updateCamera(CAMERA_FREE);
        if (_camera->position().y() < 2.5F) {
            _camera->setPosition({_camera->position().x(), 2.5F, _camera->position().z()});
            _camera->setTarget({_camera->target().x(),
                                _camera->target().y() + (raylib::rcore::Window::frameTime() * 2.0F),
                                _camera->target().z()});
        }
        _event.update();
    }

    if (!_pauseMenu->isVisible()) {
        _skyBackground.update(raylib::rcore::Window::frameTime());
    }
}

void Render::render2D() { _skyBackground.draw(_window); }

void Render::render3D() {
    _camera->beginMode3D();
    _map.draw(*_camera);
    raylib::rcore::Camera::endMode3D();
}
}  // namespace zappy::gui::graphics
