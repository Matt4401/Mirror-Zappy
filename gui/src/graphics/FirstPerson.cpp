/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** FirstPerson
*/

#include "FirstPerson.hpp"

#include <functional>
#include <memory>
#include <utility>

#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector3.hpp"
#include "rtext/Font.hpp"
#include "scene/WorldManager.hpp"
#include "ui/UIManager.hpp"
#include "ui/components/UIButton.hpp"

namespace zappy::gui::graphics {
FirstPerson::FirstPerson(events::EventDispatcher& dispatcher, raylib::rcore::Camera& camera,
                         scene::WorldManager& worldManager, ui::UIManager& uiManager,
                         const std::shared_ptr<raylib::rtext::Font>& font, std::function<void()> onStateChanged)
    : _dispatcher(dispatcher),
      _camera(camera),
      _worldManager(worldManager),
      _exitButton(std::make_shared<ui::components::UIButton>(0.0F, ExitButtonTop, ExitButtonWidth, ExitButtonHeight,
                                                             "Exit view", font)),
      _onStateChanged(std::move(onStateChanged)) {
    _exitButton->setFontSize(ExitButtonFontSize);
    _exitButton->setVisible(false);
    _exitButton->setOnClick([this]() { exit(); });
    uiManager.addComponent(_exitButton);
    updateButtonPosition();

    _eventToken = _dispatcher.get().subscribe<events::PlayerFirstPersonRequested>(
        [this](const events::PlayerFirstPersonRequested& event) { handleRequest(event); });
}

FirstPerson::~FirstPerson() {
    if (_eventToken != 0) {
        _dispatcher.get().unsubscribe<events::PlayerFirstPersonRequested>(_eventToken);
    }
}

void FirstPerson::handleRequest(const events::PlayerFirstPersonRequested& event) {
    if (!_worldManager.get().playerById(event.playerId).has_value()) {
        return;
    }

    _playerId = event.playerId;
    if (_exitButton) {
        _exitButton->setVisible(true);
    }
    updateCamera();
    notifyStateChanged();
}

void FirstPerson::update() {
    updateButtonPosition();
    updateCamera();
}

void FirstPerson::updateCamera() {
    if (!_playerId.has_value()) {
        return;
    }
    const auto player = _worldManager.get().playerById(*_playerId);
    if (!player.has_value()) {
        exit();
        return;
    }

    const auto direction = player->get().renderDirection();
    const auto eyePosition = player->get().position() + raylib::rmath::Vector3{0.0F, EyeHeight, 0.0F};

    _camera.get().setPosition(eyePosition);
    _camera.get().setTarget(eyePosition + (direction * LookDistance));
    _camera.get().setUp({0.0F, 1.0F, 0.0F});
    _camera.get().setFovy(FovY);
    _camera.get().setProjection(PerspectiveProjection);
}

void FirstPerson::exit() {
    if (!_playerId.has_value()) {
        return;
    }
    _playerId.reset();
    if (_exitButton) {
        _exitButton->setVisible(false);
    }
    notifyStateChanged();
}

void FirstPerson::updateButtonPosition() const {
    if (!_exitButton) {
        return;
    }
    const float buttonX = (static_cast<float>(raylib::rcore::Window::screenWidth()) - ExitButtonWidth) / 2.0F;
    _exitButton->setPosition(buttonX, ExitButtonTop);
}

void FirstPerson::notifyStateChanged() const {
    if (_onStateChanged) {
        _onStateChanged();
    }
}
}  // namespace zappy::gui::graphics
