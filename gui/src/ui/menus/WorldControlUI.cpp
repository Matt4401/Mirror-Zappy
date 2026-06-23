/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** WorldControlUI
*/

#include "WorldControlUI.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <ios>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "components/UIButton.hpp"
#include "components/UIDropdown.hpp"
#include "components/UISlider.hpp"
#include "components/UIText.hpp"
#include "events/GuiEvents.hpp"
#include "protocol/Commands.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Rectangle.hpp"
#include "rmath/Vector2.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIGamePanel.hpp"

namespace zappy::gui::ui::menus {

WorldControlUI::WorldControlUI(float x, float width, std::shared_ptr<events::EventDispatcher> dispatcher,
                               std::shared_ptr<raylib::rtext::Font> font,
                               std::function<void(const std::string&)> onSendCommand)
    : components::UIGamePanel(x, 0.0F, width, ContentHeight + FoldBtnHeight, "World Control"),
      _dispatcher(std::move(dispatcher)),
      _font(std::move(font)),
      _onSendCommand(std::move(onSendCommand)),
      _currentHeight(0.0F) {
    _foldBtn = std::make_shared<components::UIButton>(0.0F, 0.0F, FoldBtnWidth, FoldBtnHeight, "^", _font);
    _foldBtn->setFontSize(FoldBtnFontSize);
    _foldBtn->setOnClick([this]() { _isFolded = !_isFolded; });

    initSpeedControls();
    initTimeControls();
    initEventSubscriptions();

    updateChildrenPositions();
    setResizable(false);
}

void WorldControlUI::initSpeedControls() {
    _speedTitle = std::make_shared<components::UIText>("Server Frequency:", _font);
    _speedTitle->setFontSize(DefaultFontSize);
    _speedTitle->setColor(raylib::Color::Black());

    _speedValueText = std::make_shared<components::UIText>("100", _font);
    _speedValueText->setFontSize(DefaultFontSize);
    _speedValueText->setColor(raylib::Color::Black());

    _speedSlider = std::make_shared<components::UISlider>(0.0F, 0.0F, SliderWidth, SliderHeight, SliderMin, SliderMax,
                                                          SliderDefault, raylib::Color(120, 120, 120, 255),
                                                          raylib::Color(50, 50, 50, 255));
    _speedSlider->setOnValueChanged([this](float value) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(0) << value;
        _speedValueText->setText(ss.str());

        if (_onSendCommand) {
            _onSendCommand("sst " + ss.str() + "\n");
        }
    });
}

void WorldControlUI::initTimeControls() {
    _timeTitle = std::make_shared<components::UIText>("Time of Day:", _font);
    _timeTitle->setFontSize(DefaultFontSize);
    _timeTitle->setColor(raylib::Color::Black());

    const std::vector<std::string> options{"morning", "afternoon", "evening", "night", "cycle"};
    _timeDropdown = std::make_shared<components::UIDropdown>(0.0F, 0.0F, DropdownWidth, DropdownHeight, options, _font);
    _timeDropdown->setDirection(components::UIDropdown::Direction::UP);
    _timeDropdown->setSelectedIndex(4);

    _timeDropdown->setOnSelect([this](const std::string& option) {
        if (!_dispatcher) {
            return;
        }
        events::TimeMode mode = events::TimeMode::CYCLE;
        if (option == "morning") {
            mode = events::TimeMode::MORNING;
        } else if (option == "afternoon") {
            mode = events::TimeMode::AFTERNOON;
        } else if (option == "evening") {
            mode = events::TimeMode::EVENING;
        } else if (option == "night") {
            mode = events::TimeMode::NIGHT;
        }
        _dispatcher->dispatch(events::TimeOfDayChanged{mode});
    });
}

void WorldControlUI::initEventSubscriptions() {
    if (_dispatcher) {
        _eventTokens.push_back(
            _dispatcher->subscribe<shared::protocol::server::Sgt>([this](const shared::protocol::server::Sgt& cmd) {
                _serverFreq = static_cast<float>(cmd.timeUnit);
                _speedSlider->setValue(_serverFreq);
                std::stringstream ss;
                ss << std::fixed << std::setprecision(0) << _serverFreq;
                _speedValueText->setText(ss.str());
            }));
    }
}

WorldControlUI::~WorldControlUI() {
    if (_dispatcher) {
        for (auto token : _eventTokens) {
            _dispatcher->unsubscribe<shared::protocol::server::Sgt>(token);
        }
    }
}

void WorldControlUI::updateChildrenPositions() {
    if (_foldBtn) {
        _foldBtn->setPosition(getPosition().x() + ((getSize().x() - FoldBtnWidth) / 2.0F), getPosition().y());
    }

    float const currentY = getPosition().y() + FoldBtnHeight + ((ContentHeight - DropdownHeight) / 2.0F);
    float currentX = getPosition().x() + InitialOffsetX;

    if (_timeTitle) {
        _timeTitle->setPosition(currentX, currentY + TitleOffsetY);
        currentX += TimeTitleWidth;
    }
    if (_timeDropdown) {
        _timeDropdown->setPosition(currentX, currentY);
        currentX += TimeDropdownWidth;
    }

    if (_speedTitle) {
        _speedTitle->setPosition(currentX, currentY + TitleOffsetY);
        currentX += SpeedTitleWidth;
    }
    if (_speedSlider) {
        _speedSlider->setPosition(currentX, currentY + SliderOffsetY);
        currentX += SpeedSliderWidth;
    }
    if (_speedValueText) {
        _speedValueText->setPosition(currentX, currentY + TitleOffsetY);
    }
}

void WorldControlUI::setPosition(float x, float y) {
    setBasePosition(x, y);
    updateChildrenPositions();
}

void WorldControlUI::setSize(float width, float height) {
    setBaseSize(width, height);
    updateChildrenPositions();
}

bool WorldControlUI::isHovered() const {
    if (!isVisible()) {
        return false;
    }
    raylib::rmath::Vector2 const mousePos = raylib::rcore::Event::getMousePositionStatic();

    raylib::rmath::Rectangle const foldRec{.x = getPosition().x() + ((getSize().x() - FoldBtnWidth) / 2.0F),
                                           .y = getPosition().y(),
                                           .width = FoldBtnWidth,
                                           .height = FoldBtnHeight};
    if (raylib::rshapes::Shapes::checkCollisionPointRec(mousePos, foldRec)) {
        return true;
    }

    if (_currentHeight > FoldAnimThreshold) {
        raylib::rmath::Rectangle const contentRec{.x = getPosition().x(),
                                                  .y = getPosition().y() + FoldBtnHeight,
                                                  .width = getSize().x(),
                                                  .height = _currentHeight};
        if (raylib::rshapes::Shapes::checkCollisionPointRec(mousePos, contentRec)) {
            return true;
        }

        if (_timeDropdown && _timeDropdown->isOpen()) {
            raylib::rmath::Vector2 const ddPos = _timeDropdown->getPosition();
            raylib::rmath::Vector2 const ddSize = _timeDropdown->getSize();
            float const expandedHeight = ddSize.y() * static_cast<float>(_timeDropdown->getOptionsCount());
            raylib::rmath::Rectangle const dropdownRec{
                .x = ddPos.x(), .y = ddPos.y() - expandedHeight, .width = ddSize.x(), .height = expandedHeight};
            if (raylib::rshapes::Shapes::checkCollisionPointRec(mousePos, dropdownRec)) {
                return true;
            }
        }
    }
    return false;
}

void WorldControlUI::update() {
    if (!isVisible()) {
        return;
    }

    if (isConfigMode() && _isFolded) {
        _isFolded = false;
    }

    float const targetHeight = _isFolded ? 0.0F : ContentHeight;
    float const lerpSpeed = LerpSpeedMultiplier * raylib::rcore::Window::frameTime();

    _currentHeight += (targetHeight - _currentHeight) * lerpSpeed;
    if (std::abs(_currentHeight - targetHeight) < FoldAnimThreshold) {
        _currentHeight = targetHeight;
    }

    if (!isDragged()) {
        auto const screenHeight = static_cast<float>(raylib::rcore::Window::screenHeight());
        float const lockedY = screenHeight - (_currentHeight + FoldBtnHeight);
        setPosition(getPosition().x(), lockedY);
    }

    if (_foldBtn) {
        _foldBtn->setText(_isFolded ? "^" : "v");
        _foldBtn->update();
    }

    if (_currentHeight > MinUpdateHeight) {
        if (_speedSlider) {
            _speedSlider->update();
        }
        if (_timeDropdown) {
            _timeDropdown->update();
        }
    }
}

void WorldControlUI::handleEvent() {
    if (!isVisible()) {
        return;
    }

    if (isConfigMode()) {
        if (raylib::rcore::Event::isMouseButtonPressed(0) && isHovered()) {
            _isDragging = true;
        }
        if (raylib::rcore::Event::isMouseButtonReleased(0)) {
            _isDragging = false;
        }
        if (_isDragging) {
            float const dx = raylib::rcore::Event::getMouseDeltaStatic().x();
            auto const screenWidth = static_cast<float>(raylib::rcore::Window::screenWidth());
            setPosition(std::clamp(getPosition().x() + dx, 0.0F, screenWidth - getSize().x()), getPosition().y());
            return;
        }
    }

    if (_foldBtn) {
        _foldBtn->handleEvent();
    }

    if (_currentHeight > MinUpdateHeight) {
        if (_timeDropdown) {
            _timeDropdown->handleEvent();
        }
        if (_speedSlider) {
            _speedSlider->handleEvent();
        }
    }
}

void WorldControlUI::draw() {
    if (!isVisible()) {
        return;
    }

    if (_foldBtn) {
        _foldBtn->draw();
    }

    if (_currentHeight > MinDrawHeight) {
        raylib::rmath::Rectangle const contentRec{.x = getPosition().x(),
                                                  .y = getPosition().y() + FoldBtnHeight,
                                                  .width = getSize().x(),
                                                  .height = _currentHeight};
        raylib::rshapes::Shapes::drawRectangleRec(contentRec, raylib::Color(224, 224, 224, 255));

        raylib::rcore::Window::beginScissorMode(static_cast<int>(getPosition().x()),
                                                static_cast<int>(getPosition().y() + FoldBtnHeight),
                                                static_cast<int>(getSize().x()), static_cast<int>(_currentHeight));

        if (!isConfigMode()) {
            if (_speedTitle) {
                _speedTitle->draw();
            }
            if (_speedValueText) {
                _speedValueText->draw();
            }
            if (_speedSlider) {
                _speedSlider->draw();
            }
            if (_timeTitle) {
                _timeTitle->draw();
            }
        }

        raylib::rcore::Window::endScissorMode();

        if (!isConfigMode() && _timeDropdown) {
            _timeDropdown->draw();
        }
    }
}

}  // namespace zappy::gui::ui::menus
