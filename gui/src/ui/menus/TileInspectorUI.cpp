/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** TileInspectorUI
*/

#include "TileInspectorUI.hpp"

#include <raylib.h>

#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "AInspectorUI.hpp"
#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "GuiEvents.hpp"
#include "components/UIGamePanel.hpp"
#include "components/UIText.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"
#include "rcore/Window.hpp"
#include "rmodels/Model.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::menus {

namespace {
constexpr float HeaderFontSize = 20.0F;
constexpr float InfoFontSize = 16.0F;
constexpr float BaseYOffset = 60.0F;
constexpr float BoxHeight = 30.0F;
constexpr float BoxYOffset = 5.0F;
constexpr float BoxXOffset = 10.0F;
constexpr float BoxWidthRatio = 0.6F;
constexpr float AvatarWidth = 100.0F;
constexpr float AvatarHeight = 140.0F;
constexpr float ScannerYOffset = 30.0F;
}  // namespace

TileInspectorUI::TileInspectorUI(float x, float y, float width, std::shared_ptr<events::EventDispatcher> dispatcher,
                                 const std::shared_ptr<raylib::rtext::Font>& font,
                                 std::function<void(const std::string&)> onSendCommand)
    : AInspectorUI(x, y, width, "Tile Inspector", std::move(dispatcher), font, std::move(onSendCommand)) {
    buildInventoryPanel();
    buildScannerPanel();

    getPreviewModel() = std::make_shared<raylib::rmodels::Model>("assets/jeffrey/scene.gltf");

    if (getDispatcher()) {
        getEventTokens().push_back(getDispatcher()->subscribe<events::TileClicked>(
            [this](const events::TileClicked& e) { onTileClicked(e); }));
        getEventTokens().push_back(getDispatcher()->subscribe<events::PlayerClicked>(
            [this](const events::PlayerClicked& /*e*/) { setVisible(false); }));
        getEventTokens().push_back(getDispatcher()->subscribe<shared::protocol::server::Bct>(
            [this](const shared::protocol::server::Bct& cmd) { onBctReceived(cmd); }));
    }
}

TileInspectorUI::~TileInspectorUI() {
    if (getDispatcher() && getEventTokens().size() >= 3) {
        getDispatcher()->unsubscribe<events::TileClicked>(getEventTokens().at(0));
        getDispatcher()->unsubscribe<events::PlayerClicked>(getEventTokens().at(1));
        getDispatcher()->unsubscribe<shared::protocol::server::Bct>(getEventTokens().at(2));
    }
}

void TileInspectorUI::buildScannerPanel() {
    _titleText = std::make_shared<components::UIText>("Tile Info", getFont());
    _titleText->setFontSize(static_cast<int>(HeaderFontSize));
    _titleText->setColor(raylib::Color::White());

    _posText = std::make_shared<components::UIText>("(0, 0)", getFont());
    _posText->setFontSize(static_cast<int>(InfoFontSize));
    _posText->setColor(raylib::Color::DarkGray());

    _scannerTitle = std::make_shared<components::UIText>("Incantation Scanner", getFont());
    _scannerTitle->setFontSize(static_cast<int>(HeaderFontSize));
    _scannerTitle->setColor(raylib::Color::Black());

    _scannerInfo = std::make_shared<components::UIText>("No ritual active.\nPlayers on tile: 0", getFont());
    _scannerInfo->setFontSize(static_cast<int>(InfoFontSize));
    _scannerInfo->setColor(raylib::Color::DarkGray());
}

void TileInspectorUI::onTileClicked(const events::TileClicked& event) {
    _targetGridX = event.x;
    _targetGridY = event.y;

    if (_posText) {
        _posText->setText("Grid Pos: (" + std::to_string(_targetGridX) + ", " + std::to_string(_targetGridY) + ")");
    }

    setVisible(true);
    requestTileSync();
}

void TileInspectorUI::onBctReceived(const shared::protocol::server::Bct& cmd) {
    if (cmd.x == _targetGridX && cmd.y == _targetGridY) {
        if (getInventoryTexts().size() >= 7) {
            getInventoryTexts().at(0)->setText(std::to_string(cmd.food));
            getInventoryTexts().at(1)->setText(std::to_string(cmd.linemate));
            getInventoryTexts().at(2)->setText(std::to_string(cmd.deraumere));
            getInventoryTexts().at(3)->setText(std::to_string(cmd.sibur));
            getInventoryTexts().at(4)->setText(std::to_string(cmd.mendiane));
            getInventoryTexts().at(5)->setText(std::to_string(cmd.phiras));
            getInventoryTexts().at(6)->setText(std::to_string(cmd.thystame));
        }
    }
}

void TileInspectorUI::requestTileSync() const {
    if (getOnSendCommand() && _targetGridX >= 0 && _targetGridY >= 0) {
        getOnSendCommand()(shared::protocol::Emitter::build(shared::protocol::client::Bct{_targetGridX, _targetGridY}));
    }
}

void TileInspectorUI::update() { AInspectorUI::update(); }

void TileInspectorUI::setVisible(bool visible) {
    AInspectorUI::setVisible(visible);
    if (!visible) {
        _targetGridX = -1;
        _targetGridY = -1;
    }
}

void TileInspectorUI::draw() {
    draw3DPreview();

    AInspectorUI::draw();

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

    float const nameW = panelW * BoxWidthRatio;
    float const nameX = startX + ((panelW - nameW) / 2.0F);
    raylib::rshapes::Shapes::drawRectangleRec({.x = nameX, .y = currentY, .width = nameW, .height = BoxHeight},
                                              raylib::Color(50, 50, 50, 255));
    _titleText->setPosition(nameX + BoxXOffset, currentY + BoxYOffset);
    _titleText->draw();
    currentY += BoxHeight + 10.0F;

    float const posTextW = static_cast<float>(_posText->text().length()) * (InfoFontSize / 2.0F);
    _posText->setPosition(startX + ((panelW - posTextW) / 2.0F), currentY);
    _posText->draw();
    currentY += 30.0F;

    float const blockX = startX + ((panelW - AvatarWidth) / 2.0F);
    raylib::rshapes::Shapes::drawRectangleRec(
        {.x = blockX, .y = currentY, .width = AvatarWidth, .height = AvatarHeight}, raylib::Color::LightGray());
    if (getPreviewRenderTexture()) {
        getPreviewRenderTexture()->draw({blockX, currentY}, raylib::Color::White());
    }
    currentY += AvatarHeight + ScannerYOffset;

    float const scannerTitleW = static_cast<float>(_scannerTitle->text().length()) * (HeaderFontSize / 2.0F);
    _scannerTitle->setPosition(startX + ((panelW - scannerTitleW) / 2.0F), currentY);
    _scannerTitle->draw();
    currentY += 30.0F;

    _scannerInfo->setPosition(startX + 20.0F, currentY);
    _scannerInfo->draw();
    currentY += 60.0F;

    drawInventoryPanel(currentY, startX, panelW);

    raylib::rcore::Window::endScissorMode();
}

}  // namespace zappy::gui::ui::menus
