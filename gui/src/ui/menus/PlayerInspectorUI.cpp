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

#include "AInspectorUI.hpp"
#include "Color.hpp"
#include "components/UIButton.hpp"
#include "components/UIGamePanel.hpp"
#include "components/UIImage.hpp"
#include "components/UIText.hpp"
#include "components/UITextbox.hpp"
#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
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

namespace zappy::gui::ui::menus {

namespace {
constexpr float FirstPersonBtnWidth = 160.0F;
constexpr float FirstPersonBtnHeight = 40.0F;
constexpr int FirstPersonBtnFontSize = 16;

constexpr float FoodMax = 126.0F;

constexpr float PrimaryIconSize = 28.0F;
constexpr float PosIconSize = 20.0F;

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

constexpr float InvToBtnSpacing = 20.0F;

constexpr float CameraPosX = 0.0F;
constexpr float CameraPosY = 4.0F;
constexpr float CameraPosZ = 10.0F;
constexpr float CameraTargetX = 0.0F;
constexpr float CameraTargetY = 3.0F;
constexpr float CameraTargetZ = 0.0F;
constexpr float CameraUpX = 0.0F;
constexpr float CameraUpY = 1.0F;
constexpr float CameraUpZ = 0.0F;
constexpr float CameraFovY = 45.0F;

constexpr float ModelPosX = 0.0F;
constexpr float ModelPosY = 0.0F;
constexpr float ModelPosZ = 0.0F;
}  // namespace

PlayerInspectorUI::PlayerInspectorUI(float x, float y, float width, events::EventDispatcher& dispatcher,
                                     const std::shared_ptr<raylib::rtext::Font>& font,
                                     std::function<void(const std::string&)> onSendCommand)
    : AInspectorUI(x, y, width, "Player Inspector", dispatcher, font, std::move(onSendCommand)),
      _firstPersonBtn(std::make_shared<components::UIButton>(0.0F, 0.0F, FirstPersonBtnWidth, FirstPersonBtnHeight,
                                                             "First Person", getFont())),
      _previewCamera(raylib::rcore::Camera(raylib::rmath::Vector3{CameraPosX, CameraPosY, CameraPosZ},
                                           raylib::rmath::Vector3{CameraTargetX, CameraTargetY, CameraTargetZ},
                                           raylib::rmath::Vector3{CameraUpX, CameraUpY, CameraUpZ}, CameraFovY, 0)),
      _previewRenderTexture(std::make_shared<raylib::rtextures::RenderTexture2D>(static_cast<int>(AvatarWidth),
                                                                                 static_cast<int>(AvatarHeight))) {
    buildInfoPanel();
    buildInventoryPanel();

    _previewModel = std::make_shared<raylib::rmodels::Model>("assets/models/player/player.gltf");

    _firstPersonBtn->setFontSize(FirstPersonBtnFontSize);
    _firstPersonBtn->setOnClick([this]() {
        if (_targetPlayerId >= 0) {
            getDispatcher().dispatch(events::PlayerFirstPersonRequested{.playerId = _targetPlayerId});
        }
    });

    getEventTokens().push_back(getDispatcher().subscribe<events::PlayerClicked>(
        [this](const events::PlayerClicked& e) { onPlayerClicked(e); }));
    getEventTokens().push_back(getDispatcher().subscribe<events::TileClicked>([](const events::TileClicked& /*e*/) {}));
    getEventTokens().push_back(getDispatcher().subscribe<shared::protocol::server::Pin>(
        [this](const shared::protocol::server::Pin& cmd) { onPinReceived(cmd); }));
    getEventTokens().push_back(getDispatcher().subscribe<shared::protocol::server::Plv>(
        [this](const shared::protocol::server::Plv& cmd) { onPlvReceived(cmd); }));
    getEventTokens().push_back(getDispatcher().subscribe<shared::protocol::server::Ppo>(
        [this](const shared::protocol::server::Ppo& cmd) { onPpoReceived(cmd); }));
}

void PlayerInspectorUI::onPinReceived(const shared::protocol::server::Pin& cmd) {
    if (cmd.playerId == _targetPlayerId) {
        _foodFloat = static_cast<float>(cmd.food);
        updateHearts();

        if (getInventoryTexts().size() >= 6) {
            getInventoryTexts().at(0)->setText(std::to_string(cmd.linemate));
            getInventoryTexts().at(1)->setText(std::to_string(cmd.deraumere));
            getInventoryTexts().at(2)->setText(std::to_string(cmd.sibur));
            getInventoryTexts().at(3)->setText(std::to_string(cmd.mendiane));
            getInventoryTexts().at(4)->setText(std::to_string(cmd.phiras));
            getInventoryTexts().at(5)->setText(std::to_string(cmd.thystame));
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

PlayerInspectorUI::~PlayerInspectorUI() {
    if (getEventTokens().size() >= 5) {
        getDispatcher().unsubscribe<events::PlayerClicked>(getEventTokens().at(0));
        getDispatcher().unsubscribe<events::TileClicked>(getEventTokens().at(1));
        getDispatcher().unsubscribe<shared::protocol::server::Pin>(getEventTokens().at(2));
        getDispatcher().unsubscribe<shared::protocol::server::Plv>(getEventTokens().at(3));
        getDispatcher().unsubscribe<shared::protocol::server::Ppo>(getEventTokens().at(4));
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

    _nameBox = std::make_shared<components::UITextbox>(0.0F, 0.0F, 200.0F, NameBoxHeight, getFont(), "Enter Name...");
    _nameBox->setMaxLength(12);
    _nameBox->setOnSubmit([this](const std::string& name) {
        if (_targetPlayerId != -1) {
            getDispatcher().dispatch(events::PlayerNameChanged{.playerId = _targetPlayerId, .newName = name});
        }
    });

    _teamText = std::make_shared<components::UIText>("None", getFont());
    _teamText->setFontSize(static_cast<int>(InfoFontSize));
    _teamText->setColor(raylib::Color::DarkGray());

    _idText = std::make_shared<components::UIText>("ID: 0", getFont());
    _idText->setFontSize(static_cast<int>(InfoFontSize));
    _idText->setColor(raylib::Color::DarkGray());

    _compassText = std::make_shared<components::UIText>("North", getFont());
    _compassText->setFontSize(static_cast<int>(InfoFontSize));
    _compassText->setColor(raylib::Color::DarkGray());

    _posText = std::make_shared<components::UIText>("(0, 0)", getFont());
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

void PlayerInspectorUI::onPlayerClicked(const events::PlayerClicked& event) {
    _targetPlayerId = event.playerId;

    if (_nameBox) {
        _nameBox->setText(event.playerName);
    }
    if (_idText) {
        _idText->setText("ID: " + std::to_string(event.playerId));
    }
    if (_teamText) {
        _teamText->setText(event.teamName);
        _teamText->setColor(event.teamColor);
    }
    if (_previewModel) {
        if (!event.textureId.empty()) {
            auto tex = graphics::AssetManager::getInstance().getTexture(event.textureId);
            if (tex && tex->valid()) {
                // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
                const int materialIndex = _previewModel->model().meshMaterial[0];
                _previewModel->setMaterialTexture(materialIndex, 0 /* MATERIAL_MAP_ALBEDO */, *tex);
            }
        } else {
            // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
            const int materialIndex = _previewModel->model().meshMaterial[0];
            _previewModel->setMaterialTexture(materialIndex, 0 /* MATERIAL_MAP_ALBEDO */,
                                              raylib::rtextures::Texture2D{::Texture2D{}, false});
        }
    }

    setVisible(true);
    requestPlayerSync();
}

void PlayerInspectorUI::requestPlayerSync() const {
    if (getOnSendCommand()) {
        getOnSendCommand()(shared::protocol::Emitter::build(shared::protocol::client::Pin{_targetPlayerId}));
        getOnSendCommand()(shared::protocol::Emitter::build(shared::protocol::client::Plv{_targetPlayerId}));
        getOnSendCommand()(shared::protocol::Emitter::build(shared::protocol::client::Ppo{_targetPlayerId}));
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

void PlayerInspectorUI::handleEvent() {
    AInspectorUI::handleEvent();
    if (!isVisible()) {
        return;
    }

    if (_targetPlayerId != -1) {
        if (raylib::rcore::Event::isKeyPressed(KEY_LEFT)) {
            getDispatcher().dispatch(events::RequestCyclePlayer{.direction = -1, .currentPlayerId = _targetPlayerId});
        } else if (raylib::rcore::Event::isKeyPressed(KEY_RIGHT)) {
            getDispatcher().dispatch(events::RequestCyclePlayer{.direction = 1, .currentPlayerId = _targetPlayerId});
        }
    }

    if (_firstPersonBtn) {
        _firstPersonBtn->handleEvent();
    }
    if (_nameBox) {
        _nameBox->handleEvent();
    }
}

void PlayerInspectorUI::setVisible(bool visible) {
    AInspectorUI::setVisible(visible);
    if (!visible) {
        _targetPlayerId = -1;
    }
}

void PlayerInspectorUI::setPosition(float x, float y) { AInspectorUI::setPosition(x, y); }

void PlayerInspectorUI::update() {
    AInspectorUI::update();

    if (_foodFloat > 0.0F && getServerFreq() > 0.0F) {
        float const deltaTime = raylib::rcore::Window::frameTime();
        _foodFloat -= deltaTime * (getServerFreq() / FoodMax);
        _foodFloat = std::max(_foodFloat, 0.0F);
        updateHearts();
    }

    if (_firstPersonBtn) {
        _firstPersonBtn->update();
    }
    if (_nameBox) {
        _nameBox->update();
    }
}

void PlayerInspectorUI::drawHeader(float& currentY, float startX, float panelW) {
    float const nameW = panelW * NameBoxWidthRatio;
    float const nameX = startX + ((panelW - nameW) / 2.0F);
    _nameBox->setPosition(nameX + NameBoxXOffset, currentY + NameBoxYOffset);
    _nameBox->setSize(nameW, NameBoxHeight);
    _nameBox->draw();
    currentY += NameToAvatarSpacing;
}

void PlayerInspectorUI::drawStatsBlock(float& currentY, float startX, float panelW) {
    float const blockW = AvatarWidth + AvatarToStatsSpacing + StatsBlockWidth;
    float const blockX = startX + ((panelW - blockW) / 2.0F);

    raylib::rshapes::Shapes::drawRectangleRec(
        {.x = blockX, .y = currentY, .width = AvatarWidth, .height = AvatarHeight}, raylib::Color::LightGray());
    if (_previewRenderTexture) {
        _previewRenderTexture->draw({blockX, currentY}, raylib::Color::White());
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
}

void PlayerInspectorUI::drawXpBar(float& currentY, float startX, float panelW) {
    float const xpW = (XpBaseSegments * XpIconSpacing) + XpBarExtraWidth;
    float xpX = startX + ((panelW - xpW) / 2.0F);
    for (auto& xp : _xpBar) {
        xp->setPosition(xpX, currentY);
        xp->draw();
        xpX += XpIconSpacing;
    }
    currentY += XpToHeartsSpacing;
}

void PlayerInspectorUI::drawHealthBar(float& currentY, float startX, float panelW) {
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
}

void PlayerInspectorUI::drawActionButtons(float& currentY, float startX, float panelW) {
    if (_firstPersonBtn) {
        float const btnX = startX + ((panelW - FirstPersonBtnWidth) / 2.0F);
        _firstPersonBtn->setPosition(btnX, currentY);
        _firstPersonBtn->draw();
        currentY += FirstPersonBtnHeight;
    }
}

void PlayerInspectorUI::draw3DPreview() {
    if (isVisible() && !isConfigMode() && _previewRenderTexture && _previewModel) {
        _previewRenderTexture->begin();
        zappy::gui::raylib::rcore::Window::clearBackground({200, 200, 200, 255});
        _previewCamera.beginMode3D();
        raylib::rmath::Vector3 const modelPos{ModelPosX + _previewModelOffset.x(), ModelPosY + _previewModelOffset.y(),
                                              ModelPosZ + _previewModelOffset.z()};
        _previewModel->drawModel(modelPos, _previewModelScale, raylib::Color::White());
        zappy::gui::raylib::rcore::Camera::endMode3D();
        zappy::gui::raylib::rtextures::RenderTexture2D::end();
    }
}

void PlayerInspectorUI::draw() {
    draw3DPreview();
    AInspectorUI::draw();

    if (!isVisible() || isConfigMode()) {
        return;
    }

    float const panelW = this->getSize().x();
    float const startX = this->getPosition().x();

    float const headerH = components::UIGamePanel::getHeaderHeight();
    float const padding = components::UIGamePanel::getPadding();
    float const currentH = this->getCurrentHeight();

    float const innerX = startX + padding;
    float const innerY = this->getPosition().y() + headerH;
    float const innerW = panelW - (2.0F * padding);
    float const innerH = currentH - headerH - padding;

    raylib::rcore::Window::beginScissorMode(static_cast<int>(innerX), static_cast<int>(innerY),
                                            static_cast<int>(innerW), static_cast<int>(innerH));

    float currentY = this->getPosition().y() + BaseYOffset - getScrollOffset();

    drawHeader(currentY, startX, panelW);
    drawStatsBlock(currentY, startX, panelW);
    drawXpBar(currentY, startX, panelW);
    drawHealthBar(currentY, startX, panelW);
    drawInventoryPanel(currentY, startX, panelW);
    currentY += InvToBtnSpacing;
    drawActionButtons(currentY, startX, panelW);

    float const contentBottomWithoutScroll = currentY + getScrollOffset();
    float const innerBottom = this->getPosition().y() + currentH - padding;
    float const calculatedMaxScroll = std::max(0.0F, contentBottomWithoutScroll - innerBottom + 20.0F);
    setMaxScroll(calculatedMaxScroll);

    raylib::rcore::Window::endScissorMode();
}

}  // namespace zappy::gui::ui::menus
