/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** AInspectorUI
*/

#include "AInspectorUI.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "components/UIButton.hpp"
#include "components/UIGamePanel.hpp"
#include "components/UIImage.hpp"
#include "components/UIText.hpp"
#include "protocol/Commands.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rcore/Window.hpp"
#include "rmath/Vector3.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"
#include "rtextures/RenderTexture2D.hpp"

namespace zappy::gui::ui::menus {

namespace {
constexpr float InspectorInitialHeight = 550.0F;

constexpr float AvatarWidth = 100.0F;
constexpr float AvatarHeight = 140.0F;

constexpr float InvGridTotalWidth = 230.0F;
constexpr float InvTitleToGridSpacing = 30.0F;
constexpr float InvCellSize = 70.0F;
constexpr float InvCellSpacing = 80.0F;
constexpr float InvCellTextOffsetX = 10.0F;
constexpr float InvCellTextOffsetY = 50.0F;
constexpr float InfoFontSize = 16.0F;
constexpr float HeaderFontSize = 20.0F;

constexpr float CloseBtnSize = 24.0F;
constexpr float CloseBtnFontSize = 16.0F;
constexpr float CloseBtnXOffset = 45.0F;
constexpr float CloseBtnYOffset = 8.0F;

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
constexpr float ModelScale = 0.1F;

constexpr float InvIconSize = 30.0F;

constexpr int ElementCount = 7;
}  // namespace

AInspectorUI::AInspectorUI(float x, float y, float width, const std::string& title,
                           std::shared_ptr<events::EventDispatcher> dispatcher,
                           const std::shared_ptr<raylib::rtext::Font>& font,
                           std::function<void(const std::string&)> onSendCommand)
    : components::UIGamePanel(x, y, width, InspectorInitialHeight, title),
      _dispatcher(std::move(dispatcher)),
      _font(font),
      _onSendCommand(std::move(onSendCommand)),
      _closeBtn(std::make_shared<components::UIButton>(x + width, y, CloseBtnSize, CloseBtnSize, "X", _font)),
      _previewCamera(raylib::rcore::Camera(raylib::rmath::Vector3{CameraPosX, CameraPosY, CameraPosZ},
                                           raylib::rmath::Vector3{CameraTargetX, CameraTargetY, CameraTargetZ},
                                           raylib::rmath::Vector3{CameraUpX, CameraUpY, CameraUpZ}, CameraFovY, 0)),
      _previewRenderTexture(std::make_shared<raylib::rtextures::RenderTexture2D>(static_cast<int>(AvatarWidth),
                                                                                 static_cast<int>(AvatarHeight))) {
    setCustomLayout(true);
    setVisible(false);

    _closeBtn->setFontSize(CloseBtnFontSize);
    _closeBtn->setOnClick([this]() { this->setVisible(false); });
    addHeaderComponent(_closeBtn);

    if (_dispatcher) {
        _eventTokens.push_back(_dispatcher->subscribe<shared::protocol::server::Sgt>(
            [this](const shared::protocol::server::Sgt& cmd) { _serverFreq = static_cast<float>(cmd.timeUnit); }));
    }
}

AInspectorUI::~AInspectorUI() {
    if (_dispatcher && !_eventTokens.empty()) {
        _dispatcher->unsubscribe<shared::protocol::server::Sgt>(_eventTokens.front());
    }
}

void AInspectorUI::buildInventoryPanel() {
    _inventoryTitleText = std::make_shared<components::UIText>("Inventory", _font);
    _inventoryTitleText->setFontSize(static_cast<int>(HeaderFontSize));
    _inventoryTitleText->setColor(raylib::Color::Black());

    for (int i = 0; i < ElementCount; ++i) {
        auto text = std::make_shared<components::UIText>("0", _font);
        text->setFontSize(static_cast<int>(InfoFontSize));
        text->setColor(raylib::Color::White());
        _inventoryTexts.push_back(text);

        auto img = std::make_shared<components::UIImage>(
            "assets/images/ui/id.png");  // TODO: Replace with actual inventory item image
        img->setSize(InvIconSize, InvIconSize);
        _inventoryImages.push_back(img);
    }
}

void AInspectorUI::drawInventoryPanel(float& currentY, float startX, float panelW) {
    float const titleX = startX + ((panelW - InvGridTotalWidth) / 2.0F);
    _inventoryTitleText->setPosition(titleX, currentY);
    _inventoryTitleText->draw();
    currentY += InvTitleToGridSpacing;

    float const gridX = startX + ((panelW - InvGridTotalWidth) / 2.0F);
    for (size_t i = 0; i < _inventoryTexts.size(); ++i) {
        const int row = static_cast<int>(i) / 3;
        const int col = static_cast<int>(i) % 3;
        auto& invText = _inventoryTexts.at(i);
        float const cellX = gridX + (static_cast<float>(col) * InvCellSpacing);
        float const cellY = currentY + (static_cast<float>(row) * InvCellSpacing);

        raylib::rshapes::Shapes::drawRectangleRec({.x = cellX, .y = cellY, .width = InvCellSize, .height = InvCellSize},
                                                  raylib::Color(200, 200, 200, 255));

        invText->setPosition(cellX + InvCellTextOffsetX, cellY + InvCellTextOffsetY);
        invText->draw();

        auto img = _inventoryImages.at(i);
        img->setPosition(cellX + InvCellTextOffsetX, cellY + InvCellTextOffsetY);
        img->draw();
    }
    currentY += InvCellSpacing * 3.0F;
}

void AInspectorUI::draw3DPreview() {
    if (isVisible() && !isConfigMode() && _previewRenderTexture && _previewModel) {
        _previewRenderTexture->begin();
        zappy::gui::raylib::rcore::Window::clearBackground({200, 200, 200, 255});
        _previewCamera.beginMode3D();
        _previewModel->drawModel({ModelPosX, ModelPosY, ModelPosZ}, ModelScale, raylib::Color::White());
        zappy::gui::raylib::rcore::Camera::endMode3D();
        zappy::gui::raylib::rtextures::RenderTexture2D::end();
    }
}

void AInspectorUI::draw() { components::UIGamePanel::draw(); }

void AInspectorUI::update() {
    components::UIGamePanel::update();
    if (_closeBtn) {
        _closeBtn->update();
    }
}

void AInspectorUI::handleEvent(const raylib::rcore::Event& event) {
    components::UIGamePanel::handleEvent(event);
    if (!isVisible()) {
        return;
    }
    if (_closeBtn) {
        _closeBtn->handleEvent(event);
    }
}

void AInspectorUI::setVisible(bool visible) { components::UIGamePanel::setVisible(visible); }

void AInspectorUI::setPosition(float x, float y) {
    components::UIGamePanel::setPosition(x, y);
    if (_closeBtn) {
        _closeBtn->setPosition(x + getSize().x() - CloseBtnXOffset, y + CloseBtnYOffset);
    }
}

}  // namespace zappy::gui::ui::menus
