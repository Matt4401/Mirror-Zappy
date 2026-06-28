/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** GameOverUI
*/

#include "GameOverUI.hpp"

#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "Color.hpp"
#include "events/EventDispatcher.hpp"
#include "protocol/Commands.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIPanel.hpp"

namespace zappy::gui::ui::menus {
namespace {
constexpr raylib::Color OVERLAY_COLOR(0, 0, 0, 150);
constexpr raylib::Color MODAL_COLOR(25, 25, 30, 240);
}  // namespace

GameOverUI::GameOverUI(events::EventDispatcher& dispatcher, const std::shared_ptr<raylib::rtext::Font>& font)
    : _dispatcher(dispatcher),
      _backgroundPanel(std::make_unique<components::UIPanel>(0, 0, 0, 0, OVERLAY_COLOR)),
      _modalPanel(std::make_unique<components::UIPanel>(0, 0, 0, 0, MODAL_COLOR)),
      _spectateBtn(std::make_unique<components::UIButton>(0.0F, 0.0F, 0.0F, 0.0F, "Spectate", font)),
      _exitBtn(std::make_unique<components::UIButton>(0.0F, 0.0F, 0.0F, 0.0F, "Quit Game", font)),
      _font(font) {
    _spectateBtn->setOnClick([this]() {
        setVisible(false);
        if (_onSpectate) {
            _onSpectate();
        }
    });
    _exitBtn->setOnClick([this]() {
        if (_onExit) {
            _onExit();
        }
    });

    _segSubscription = _dispatcher.get().subscribe<shared::protocol::server::Seg>(
        [this](const shared::protocol::server::Seg& command) {
            _winningTeam = command.teamName;
            setVisible(true);
        });

    recalculateLayout();
}

GameOverUI::~GameOverUI() {
    if (_segSubscription != 0) {
        _dispatcher.get().unsubscribe<shared::protocol::server::Seg>(_segSubscription);
    }
}

void GameOverUI::draw() {
    if (!_isVisible) {
        return;
    }

    _backgroundPanel->draw();
    _modalPanel->draw();
    _spectateBtn->draw();
    _exitBtn->draw();

    if (!_winningTeam.empty()) {
        std::string const title = "TEAM " + _winningTeam + " WINS!";
        float const textWidth = _font->measureTextEx(title, FontSize, FontSpacing).x();
        float const panelY = (static_cast<float>(raylib::rcore::Window::screenHeight()) - PanelHeight) / HalfRatio;
        raylib::rmath::Vector2 const textPos{
            (static_cast<float>(raylib::rcore::Window::screenWidth()) / HalfRatio) - (textWidth / HalfRatio),
            panelY + TextYOffset};
        _font->drawTextEx(title, textPos, FontSize, FontSpacing, raylib::Color::Gold());
    }
}

void GameOverUI::update() {
    if (!_isVisible) {
        return;
    }

    if (_lastScreenWidth != raylib::rcore::Window::screenWidth() ||
        _lastScreenHeight != raylib::rcore::Window::screenHeight()) {
        recalculateLayout();
    }

    _spectateBtn->update();
    _exitBtn->update();
}

void GameOverUI::handleEvent() {
    if (!_isVisible) {
        return;
    }

    _spectateBtn->handleEvent();
    _exitBtn->handleEvent();
}

void GameOverUI::setPosition(float x, float y) {
    (void)x;
    (void)y;
}

void GameOverUI::setSize(float width, float height) {
    (void)width;
    (void)height;
}

bool GameOverUI::isVisible() const { return _isVisible; }

void GameOverUI::setVisible(bool visible) {
    _isVisible = visible;
    if (visible) {
        recalculateLayout();
    }
}

bool GameOverUI::isHovered() const {
    if (!_isVisible) {
        return false;
    }
    return _backgroundPanel->isHovered() || _modalPanel->isHovered() || _spectateBtn->isHovered() ||
           _exitBtn->isHovered();
}

void GameOverUI::setOnExit(std::function<void()> callback) { _onExit = std::move(callback); }

void GameOverUI::setOnSpectate(std::function<void()> callback) { _onSpectate = std::move(callback); }

void GameOverUI::recalculateLayout() {
    _lastScreenWidth = raylib::rcore::Window::screenWidth();
    _lastScreenHeight = raylib::rcore::Window::screenHeight();

    auto const screenW = static_cast<float>(_lastScreenWidth);
    auto const screenH = static_cast<float>(_lastScreenHeight);

    _backgroundPanel->setPosition(0.0F, 0.0F);
    _backgroundPanel->setSize(screenW, screenH);
    _backgroundPanel->setRounded(false);

    float const panelX = (screenW - PanelWidth) / HalfRatio;
    float const panelY = (screenH - PanelHeight) / HalfRatio;

    _modalPanel->setPosition(panelX, panelY);
    _modalPanel->setSize(PanelWidth, PanelHeight);
    _modalPanel->setRounded(true);

    float const btnX = (screenW / HalfRatio) - (ButtonWidth / HalfRatio);

    float const startY = panelY + ButtonsStartYOffset;

    _spectateBtn->setPosition(btnX, startY);
    _spectateBtn->setSize(ButtonWidth, ButtonHeight);

    _exitBtn->setPosition(btnX, startY + ButtonHeight + ButtonSpacing);
    _exitBtn->setSize(ButtonWidth, ButtonHeight);
}

}  // namespace zappy::gui::ui::menus
