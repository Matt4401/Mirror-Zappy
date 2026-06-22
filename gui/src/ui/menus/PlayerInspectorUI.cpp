/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** PlayerInspectorUI
*/

#include "PlayerInspectorUI.hpp"

#include <raylib.h>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "Color.hpp"
#include "events/EventDispatcher.hpp"
#include "game/Player.hpp"
#include "graphics/AssetManager.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"
#include "rtextures/RenderTexture2D.hpp"
#include "rtextures/Texture2D.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIGamePanel.hpp"
#include "ui/components/UIImage.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {

namespace {
constexpr float InspectorInitialHeight = 550.0F;
constexpr float FirstPersonBtnWidth = 160.0F;
constexpr float FirstPersonBtnHeight = 40.0F;
constexpr int FirstPersonBtnFontSize = 16;

constexpr float FoodMax = 126.0F;

constexpr float PrimaryIconSize = 28.0F;
constexpr float PosIconSize = 20.0F;
constexpr float HeaderFontSize = 20.0F;
constexpr float InfoFontSize = 16.0F;

constexpr float BaseYOffset = 60.0F;
constexpr float NameBoxHeight = 30.0F;
constexpr float NameBoxYOffset = 5.0F;
constexpr float NameBoxXOffset = 10.0F;
constexpr float NameBoxWidthRatio = 0.6F;
constexpr float NameToAvatarSpacing = 40.0F;

constexpr float AvatarWidth = 100.0F;
constexpr float AvatarHeight = 140.0F;
constexpr float AvatarToStatsSpacing = 20.0F;
constexpr float StatsBlockWidth = 120.0F;

constexpr float StatsYOffset = 8.0F;
constexpr float StatsXOffset = 12.0F;
constexpr float StatsTextOffsetX = 30.0F;
constexpr float StatsTextOffsetY = 6.0F;
constexpr float StatsYSpacing = 32.0F;
constexpr float PosIconYOffset = 4.0F;

constexpr float AvatarToXpSpacing = 15.0F;

constexpr float XpIconSize = 24.0F;
constexpr float XpIconSpacing = 24.0F;
constexpr float XpBarExtraWidth = 70.0F;
constexpr float XpToHeartsSpacing = 30.0F;
constexpr float XpBaseSegments = 7.0F;

constexpr float HeartIconSize = 27.0F;
constexpr float HeartIconSpacing = 24.0F;
constexpr float HeartBarExtraWidth = 27.0F;
constexpr float HeartBaseSegments = 9.0F;
constexpr float HeartsToInventorySpacing = 40.0F;
constexpr int MaxHearts = 10;
constexpr int MaxXp = 8;

constexpr float InvGridTotalWidth = 230.0F;
constexpr float InvTitleToGridSpacing = 30.0F;
constexpr float InvCellSize = 70.0F;
constexpr float InvCellSpacing = 80.0F;
constexpr float InvCellTextOffsetX = 10.0F;
constexpr float InvCellTextOffsetY = 50.0F;
constexpr float InvToBtnSpacing = 180.0F;
}  // namespace

PlayerInspectorUI::PlayerInspectorUI(float x, float y, float width, std::shared_ptr<events::EventDispatcher> dispatcher,
                                     const std::shared_ptr<raylib::rtext::Font>& font,
                                     std::function<void(const std::string&)> onSendCommand)
    : components::UIGamePanel(x, y, width, InspectorInitialHeight, "Player Inspector"),
      _dispatcher(std::move(dispatcher)),
      _font(font),
      _onSendCommand(std::move(onSendCommand)),
      _firstPersonBtn(std::make_shared<components::UIButton>(0.0F, 0.0F, FirstPersonBtnWidth, FirstPersonBtnHeight,
                                                             "First Person", _font)),
      _closeBtn(std::make_shared<components::UIButton>(x + width, y, 24.0F, 24.0F, "X", _font)),
      _avatarCamera(raylib::rcore::Camera(raylib::rmath::Vector3{0.0F, 4.0F, 10.0F},
                                          raylib::rmath::Vector3{0.0F, 3.0F, 0.0F},
                                          raylib::rmath::Vector3{0.0F, 1.0F, 0.0F}, 45.0F, 0)),
      _avatarModel(std::make_shared<raylib::rmodels::Model>("assets/jeffrey/scene.gltf")),
      _avatarRenderTexture(std::make_shared<raylib::rtextures::RenderTexture2D>(static_cast<int>(AvatarWidth),
                                                                                static_cast<int>(AvatarHeight))) {
    setCustomLayout(true);
    buildInfoPanel();
    buildInventoryPanel();

    _firstPersonBtn->setFontSize(FirstPersonBtnFontSize);

    _closeBtn->setFontSize(16.0F);
    _closeBtn->setOnClick([this]() { this->setVisible(false); });
    addHeaderComponent(_closeBtn);

    if (_dispatcher) {
        _eventTokens.push_back(_dispatcher->subscribe<shared::protocol::server::Pin>(
            [this](const shared::protocol::server::Pin& cmd) { onPinReceived(cmd); }));
        _eventTokens.push_back(_dispatcher->subscribe<shared::protocol::server::Plv>(
            [this](const shared::protocol::server::Plv& cmd) { onPlvReceived(cmd); }));
        _eventTokens.push_back(_dispatcher->subscribe<shared::protocol::server::Ppo>(
            [this](const shared::protocol::server::Ppo& cmd) { onPpoReceived(cmd); }));
        _eventTokens.push_back(_dispatcher->subscribe<shared::protocol::server::Sgt>(
            [this](const shared::protocol::server::Sgt& cmd) { onSgtReceived(cmd); }));
    }

    _targetPlayerId = 1;  // Temporary default player ID for testing
}

void PlayerInspectorUI::onPinReceived(const shared::protocol::server::Pin& cmd) {
    if (cmd.playerId == _targetPlayerId) {
        _foodFloat = static_cast<float>(cmd.food);
        updateHearts();

        if (_inventoryTexts.size() >= 6) {
            _inventoryTexts.at(0)->setText(std::to_string(cmd.linemate));
            _inventoryTexts.at(1)->setText(std::to_string(cmd.deraumere));
            _inventoryTexts.at(2)->setText(std::to_string(cmd.sibur));
            _inventoryTexts.at(3)->setText(std::to_string(cmd.mendiane));
            _inventoryTexts.at(4)->setText(std::to_string(cmd.phiras));
            _inventoryTexts.at(5)->setText(std::to_string(cmd.thystame));
        }
    }
}

void PlayerInspectorUI::onPlvReceived(const shared::protocol::server::Plv& cmd) {
    if (cmd.playerId == _targetPlayerId) {
        _levelValue = cmd.level;
        updateHearts();
    }
}

void PlayerInspectorUI::onPpoReceived(const shared::protocol::server::Ppo& cmd) {
    if (cmd.playerId == _targetPlayerId) {
        if (_posText) {
            _posText->setText("(" + std::to_string(cmd.x) + ", " + std::to_string(cmd.y) + ")");
        }
        _orientationValue = cmd.orientation;
        if (_compassText) {
            std::string dir = "North";
            if (_orientationValue == 2) {
                dir = "East";
            } else if (_orientationValue == 3) {
                dir = "South";
            } else if (_orientationValue == 4) {
                dir = "West";
            }
            _compassText->setText(dir);
        }
    }
}

void PlayerInspectorUI::onSgtReceived(const shared::protocol::server::Sgt& cmd) {
    _serverFreq = static_cast<float>(cmd.timeUnit);
}

PlayerInspectorUI::~PlayerInspectorUI() {
    if (_dispatcher && _eventTokens.size() >= 4) {
        _dispatcher->unsubscribe<shared::protocol::server::Pin>(_eventTokens.at(0));
        _dispatcher->unsubscribe<shared::protocol::server::Plv>(_eventTokens.at(1));
        _dispatcher->unsubscribe<shared::protocol::server::Ppo>(_eventTokens.at(2));
        _dispatcher->unsubscribe<shared::protocol::server::Sgt>(_eventTokens.at(3));
    }
}

void PlayerInspectorUI::buildInfoPanel() {
    _teamIcon = std::make_shared<components::UIImage>("assets/images/ui/team.png");
    _teamIcon->setSize(PrimaryIconSize, PrimaryIconSize);
    _idIcon = std::make_shared<components::UIImage>("assets/images/ui/id.png");
    _idIcon->setSize(PrimaryIconSize, PrimaryIconSize);
    _posIcon = std::make_shared<components::UIImage>("assets/images/ui/position.png");
    _posIcon->setSize(PosIconSize, PosIconSize);
    _levelIcon = std::make_shared<components::UIImage>("assets/images/ui/id.png");
    _levelIcon->setSize(PrimaryIconSize, PrimaryIconSize);
    _compassIcon = std::make_shared<components::UIImage>("assets/images/ui/compass.png");
    _compassIcon->setSize(PrimaryIconSize, PrimaryIconSize);

    _nameText = std::make_shared<components::UIText>("Player Name", _font);
    _nameText->setFontSize(static_cast<int>(HeaderFontSize));
    _nameText->setColor(raylib::Color::White());

    _teamText = std::make_shared<components::UIText>("Team: None", _font);
    _teamText->setFontSize(static_cast<int>(InfoFontSize));
    _teamText->setColor(raylib::Color::DarkGray());

    _idText = std::make_shared<components::UIText>("ID: 0", _font);
    _idText->setFontSize(static_cast<int>(InfoFontSize));
    _idText->setColor(raylib::Color::DarkGray());

    _compassText = std::make_shared<components::UIText>("North", _font);
    _compassText->setFontSize(static_cast<int>(InfoFontSize));
    _compassText->setColor(raylib::Color::DarkGray());

    _posText = std::make_shared<components::UIText>("(0, 0)", _font);
    _posText->setFontSize(static_cast<int>(InfoFontSize));
    _posText->setColor(raylib::Color::DarkGray());

    for (int i = 0; i < MaxXp; ++i) {
        auto xp = std::make_shared<components::UIImage>("assets/images/ui/empty_xp.png");
        xp->setSize(XpIconSize, XpIconSize);
        _xpBar.push_back(xp);
    }

    for (int i = 0; i < MaxHearts; ++i) {
        auto baseHeart = std::make_shared<components::UIImage>("assets/images/ui/hearts/full_empty.png");
        baseHeart->setSize(HeartIconSize, HeartIconSize);
        _baseHearts.push_back(baseHeart);

        auto overlapHeart = std::make_shared<components::UIImage>("assets/images/ui/hearts/full_empty.png");
        overlapHeart->setSize(HeartIconSize, HeartIconSize);
        overlapHeart->setVisible(false);
        _overlapHearts.push_back(overlapHeart);
    }
}

void PlayerInspectorUI::buildInventoryPanel() {
    _inventoryTitleText = std::make_shared<components::UIText>("Inventory", _font);
    _inventoryTitleText->setFontSize(static_cast<int>(HeaderFontSize));
    _inventoryTitleText->setColor(raylib::Color::Black());

    for (int i = 0; i < 6; ++i) {
        auto text = std::make_shared<components::UIText>("0", _font);
        text->setFontSize(static_cast<int>(InfoFontSize));
        text->setColor(raylib::Color::White());
        _inventoryTexts.push_back(text);

        auto img = std::make_shared<components::UIImage>("assets/images/ui/id.png");
        img->setSize(30.0F, 30.0F);
        _inventoryImages.push_back(img);
    }
}

void PlayerInspectorUI::setTargetPlayer(int playerId, const game::Player& initialData) {
    _targetPlayerId = playerId;

    if (_nameText) {
        _nameText->setText(initialData.name());
    }
    if (_idText) {
        _idText->setText("ID: " + std::to_string(playerId));
    }
    if (_teamText) {
        _teamText->setText("Team: " + initialData.teamName());
        _teamText->setColor(initialData.teamColor());
    }
    if (_avatarModel) {
        if (!initialData.textureId().empty()) {
            auto tex = graphics::AssetManager::getInstance().getTexture(initialData.textureId());
            if (tex) {
                _avatarModel->setMaterialTexture(0, 0 /* MATERIAL_MAP_ALBEDO */, *tex);
            }
        } else {
            _avatarModel->setMaterialTexture(0, 0 /* MATERIAL_MAP_ALBEDO */,
                                             raylib::rtextures::Texture2D{::Texture2D{}, false});
        }
    }

    requestPlayerSync();
}

void PlayerInspectorUI::requestPlayerSync() const {
    if (_onSendCommand) {
        _onSendCommand(shared::protocol::Emitter::build(shared::protocol::client::Pin{_targetPlayerId}));
        _onSendCommand(shared::protocol::Emitter::build(shared::protocol::client::Plv{_targetPlayerId}));
        _onSendCommand(shared::protocol::Emitter::build(shared::protocol::client::Ppo{_targetPlayerId}));
    }
}

void PlayerInspectorUI::updateHearts() {
    int const fullHearts = static_cast<int>(_foodFloat);
    bool const hasHalfHeart = (_foodFloat - static_cast<float>(fullHearts)) >= 0.5F;

    int const row = fullHearts / 10;
    int const remainder = fullHearts % 10;

    std::string baseColorPath = "assets/images/ui/hearts/full_red.png";
    std::string overlapColorPath = "assets/images/ui/hearts/full_yellow.png";
    std::string halfOverlapColorPath = "assets/images/ui/hearts/half_yellow.png";

    switch (row) {
        case 0:
            baseColorPath = "assets/images/ui/hearts/full_empty.png";
            overlapColorPath = "assets/images/ui/hearts/full_red.png";
            halfOverlapColorPath = "assets/images/ui/hearts/half_red.png";
            break;
        case 1:
            baseColorPath = "assets/images/ui/hearts/full_red.png";
            overlapColorPath = "assets/images/ui/hearts/full_yellow.png";
            halfOverlapColorPath = "assets/images/ui/hearts/half_yellow.png";
            break;
        case 2:
            baseColorPath = "assets/images/ui/hearts/full_yellow.png";
            overlapColorPath = "assets/images/ui/hearts/full_orange.png";
            halfOverlapColorPath = "assets/images/ui/hearts/half_orange.png";
            break;
        case 3:
            baseColorPath = "assets/images/ui/hearts/full_orange.png";
            overlapColorPath = "assets/images/ui/hearts/full_green.png";
            halfOverlapColorPath = "assets/images/ui/hearts/half_green.png";
            break;
        case 4:
            baseColorPath = "assets/images/ui/hearts/full_green.png";
            overlapColorPath = "assets/images/ui/hearts/full_blue.png";
            halfOverlapColorPath = "assets/images/ui/hearts/half_blue.png";
            break;
        default:
            baseColorPath = "assets/images/ui/hearts/full_blue.png";
            overlapColorPath = "assets/images/ui/hearts/full_purple.png";
            halfOverlapColorPath = "assets/images/ui/hearts/half_purple.png";
            break;
    }

    for (int i = 0; i < MaxHearts; ++i) {
        auto& baseH = _baseHearts.at(static_cast<size_t>(i));
        auto& overH = _overlapHearts.at(static_cast<size_t>(i));

        if (baseH->getPath() != baseColorPath) {
            baseH->setTexture(baseColorPath);
        }
        baseH->setSize(HeartIconSize, HeartIconSize);

        if (i < remainder) {
            if (overH->getPath() != overlapColorPath) {
                overH->setTexture(overlapColorPath);
            }
            overH->setSize(HeartIconSize, HeartIconSize);
            overH->setVisible(true);
        } else if (i == remainder && hasHalfHeart) {
            if (overH->getPath() != halfOverlapColorPath) {
                overH->setTexture(halfOverlapColorPath);
            }
            overH->setSize(HeartIconSize, HeartIconSize);
            overH->setVisible(true);
        } else {
            overH->setVisible(false);
        }
    }

    for (int i = 0; i < MaxXp; ++i) {
        auto& xp = _xpBar.at(static_cast<size_t>(i));
        if (i < _levelValue) {
            if (xp->getPath() != "assets/images/ui/full_xp.png") {
                xp->setTexture("assets/images/ui/full_xp.png");
            }
        } else {
            if (xp->getPath() != "assets/images/ui/empty_xp.png") {
                xp->setTexture("assets/images/ui/empty_xp.png");
            }
        }
        xp->setSize(XpIconSize, XpIconSize);
    }
}

void PlayerInspectorUI::handleEvent(const raylib::rcore::Event& event) {
    components::UIGamePanel::handleEvent(event);
    if (!isVisible()) {
        return;
    }

    if (_firstPersonBtn) {
        _firstPersonBtn->handleEvent(event);
    }
    if (_closeBtn) {
        _closeBtn->handleEvent(event);
    }
}

void PlayerInspectorUI::setVisible(bool visible) {
    components::UIGamePanel::setVisible(visible);
    if (!visible) {
        _targetPlayerId = -1;
    }
}

void PlayerInspectorUI::setPosition(float x, float y) {
    components::UIGamePanel::setPosition(x, y);
    if (_closeBtn) {
        _closeBtn->setPosition(x + getSize().x() - 45.0F, y + 8.0F);
    }
}

void PlayerInspectorUI::update() {
    components::UIGamePanel::update();

    if (_foodFloat > 0.0F && _serverFreq > 0.0F) {
        float const deltaTime = raylib::rcore::Window::frameTime();
        _foodFloat -= deltaTime * (_serverFreq / FoodMax);
        _foodFloat = std::max(_foodFloat, 0.0F);
        updateHearts();
    }

    if (_firstPersonBtn) {
        _firstPersonBtn->update();
    }
    if (_closeBtn) {
        _closeBtn->update();
    }
}

void PlayerInspectorUI::draw() {
    if (isVisible() && !isConfigMode() && _avatarRenderTexture && _avatarModel) {
        _avatarRenderTexture->begin();
        zappy::gui::raylib::rcore::Window::clearBackground({200, 200, 200, 255});
        _avatarCamera.beginMode3D();
        _avatarModel->drawModel({0.0F, 0.0F, 0.0F}, 0.1F, raylib::Color::White());
        zappy::gui::raylib::rcore::Camera::endMode3D();
        zappy::gui::raylib::rtextures::RenderTexture2D::end();
    }

    components::UIGamePanel::draw();

    if (!isVisible() || isConfigMode()) {
        return;
    }

    float const panelW = this->getSize().x();
    float const startX = this->getPosition().x();

    float const headerH = components::UIGamePanel::getHeaderHeight();
    float const currentH = this->getCurrentHeight();
    raylib::rcore::Window::beginScissorMode(static_cast<int>(startX),
                                            static_cast<int>(this->getPosition().y() + headerH),
                                            static_cast<int>(panelW), static_cast<int>(currentH - headerH));

    float currentY = this->getPosition().y() + BaseYOffset;

    float const nameW = panelW * NameBoxWidthRatio;
    float const nameX = startX + ((panelW - nameW) / 2.0F);
    raylib::rshapes::Shapes::drawRectangleRec({.x = nameX, .y = currentY, .width = nameW, .height = NameBoxHeight},
                                              raylib::Color(50, 50, 50, 255));
    _nameText->setPosition(nameX + NameBoxXOffset, currentY + NameBoxYOffset);
    _nameText->draw();
    currentY += NameToAvatarSpacing;

    float const blockW = AvatarWidth + AvatarToStatsSpacing + StatsBlockWidth;
    float const blockX = startX + ((panelW - blockW) / 2.0F);

    raylib::rshapes::Shapes::drawRectangleRec(
        {.x = blockX, .y = currentY, .width = AvatarWidth, .height = AvatarHeight}, raylib::Color::LightGray());
    if (_avatarRenderTexture) {
        _avatarRenderTexture->draw({blockX, currentY}, raylib::Color::White());
    }

    float statsY = currentY + StatsYOffset;
    float const statsX = blockX + AvatarWidth + StatsXOffset;

    _teamIcon->setPosition(statsX, statsY);
    _teamIcon->draw();
    _teamText->setPosition(statsX + StatsTextOffsetX, statsY + StatsTextOffsetY);
    _teamText->draw();

    statsY += StatsYSpacing;
    _levelIcon->setPosition(statsX, statsY);
    _levelIcon->draw();
    if (_idText) {
        _idText->setPosition(statsX + StatsTextOffsetX, statsY + StatsTextOffsetY);
        _idText->draw();
    }

    statsY += StatsYSpacing;
    _compassIcon->setPosition(statsX, statsY);
    _compassIcon->draw();
    _compassText->setPosition(statsX + StatsTextOffsetX, statsY + StatsTextOffsetY);
    _compassText->draw();

    statsY += StatsYSpacing;
    _posIcon->setPosition(statsX, statsY + PosIconYOffset);
    _posIcon->draw();
    _posText->setPosition(statsX + StatsTextOffsetX, statsY + StatsTextOffsetY);
    _posText->draw();

    currentY += AvatarHeight + AvatarToXpSpacing;

    float const xpW = (XpBaseSegments * XpIconSpacing) + XpBarExtraWidth;
    float xpX = startX + ((panelW - xpW) / 2.0F);
    for (auto& xp : _xpBar) {
        xp->setPosition(xpX, currentY);
        xp->draw();
        xpX += XpIconSpacing;
    }
    currentY += XpToHeartsSpacing;

    float const heartW = (HeartBaseSegments * HeartIconSpacing) + HeartBarExtraWidth;
    float heartX = startX + ((panelW - heartW) / 2.0F);
    for (int i = 0; i < MaxHearts; ++i) {
        auto& baseH = _baseHearts.at(static_cast<size_t>(i));
        auto& overH = _overlapHearts.at(static_cast<size_t>(i));

        baseH->setPosition(heartX, currentY);
        baseH->draw();
        if (overH->isVisible()) {
            overH->setPosition(heartX, currentY);
            overH->draw();
        }
        heartX += HeartIconSpacing;
    }
    currentY += HeartsToInventorySpacing;

    float const titleX = startX + ((panelW - InvGridTotalWidth) / 2.0F);
    _inventoryTitleText->setPosition(titleX, currentY);
    _inventoryTitleText->draw();
    currentY += InvTitleToGridSpacing;

    float const gridX = startX + ((panelW - InvGridTotalWidth) / 2.0F);
    for (size_t i = 0; i < _inventoryTexts.size(); ++i) {
        const int row = static_cast<int>(i) / 3;
        const int col = static_cast<int>(i) % 3;
        auto& _inventoryText = _inventoryTexts.at(i);
        float const cellX = gridX + (static_cast<float>(col) * InvCellSpacing);
        float const cellY = currentY + (static_cast<float>(row) * InvCellSpacing);

        raylib::rshapes::Shapes::drawRectangleRec({.x = cellX, .y = cellY, .width = InvCellSize, .height = InvCellSize},
                                                  raylib::Color(200, 200, 200, 255));

        _inventoryText->setPosition(cellX + InvCellTextOffsetX, cellY + InvCellTextOffsetY);
        _inventoryText->draw();

        auto img = _inventoryImages.at(i);
        img->setPosition(cellX + InvCellTextOffsetX, cellY + InvCellTextOffsetY);
        img->draw();
    }
    currentY += InvToBtnSpacing;

    if (_firstPersonBtn) {
        float const btnX = startX + ((panelW - FirstPersonBtnWidth) / 2.0F);
        _firstPersonBtn->setPosition(btnX, currentY);
        _firstPersonBtn->draw();
    }

    raylib::rcore::Window::endScissorMode();
}

}  // namespace zappy::gui::ui::menus
