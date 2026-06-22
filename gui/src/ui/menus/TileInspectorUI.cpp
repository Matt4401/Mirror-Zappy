/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** TileInspectorUI
*/

#include "TileInspectorUI.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "AInspectorUI.hpp"
#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "GuiEvents.hpp"
#include "components/UIGamePanel.hpp"
#include "components/UIImage.hpp"
#include "components/UIText.hpp"
#include "protocol/Commands.hpp"
#include "protocol/Emitter.hpp"
#include "rcore/Window.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::menus {

namespace {
constexpr float HeaderFontSize = 20.0F;
constexpr float InfoFontSize = 16.0F;
constexpr float BaseYOffset = 60.0F;
constexpr float ElementSpacingSmall = 30.0F;
constexpr float ElementSpacingMedium = 40.0F;
constexpr float ElementSpacingLarge = 60.0F;

constexpr float FoodBoxWidth = 100.0F;
constexpr float FoodBoxHeight = 40.0F;
constexpr float FoodIconOffset = 10.0F;
constexpr float FoodIconSize = 30.0F;
constexpr float FoodTextOffset = 50.0F;

constexpr float GridCellSize = 70.0F;
constexpr float GridCellSpacing = 80.0F;
constexpr float GridTotalWidth = 230.0F;
constexpr float GridIconOffsetX = 10.0F;
constexpr float GridIconOffsetY = 50.0F;

constexpr float FoodIconDrawOffset = 5.0F;
constexpr float BoxMarginAdjustment = 10.0F;
constexpr float DoubleSpacingMultiplier = 2.0F;

constexpr size_t InventoryStartIndex = 1;
constexpr size_t InventoryEndIndex = 6;
constexpr int GridColumns = 3;
constexpr size_t TotalInventoryItems = 7;
constexpr size_t ExpectedEventTokens = 5;

constexpr float CenterDivisor = 2.0F;

constexpr raylib::Color BoxBgColor(200, 200, 200, 255);
}  // namespace

TileInspectorUI::TileInspectorUI(float x, float y, float width, std::shared_ptr<events::EventDispatcher> dispatcher,
                                 const std::shared_ptr<raylib::rtext::Font>& font,
                                 std::function<void(const std::string&)> onSendCommand)
    : AInspectorUI(x, y, width, "Tile Inspector", std::move(dispatcher), font, std::move(onSendCommand)) {
    buildInventoryPanel();
    buildScannerPanel();

    if (getDispatcher()) {
        getEventTokens().push_back(getDispatcher()->subscribe<events::TileClicked>(
            [this](const events::TileClicked& e) { onTileClicked(e); }));
        getEventTokens().push_back(getDispatcher()->subscribe<events::PlayerClicked>(
            [this](const events::PlayerClicked& /*e*/) { setVisible(false); }));
        getEventTokens().push_back(getDispatcher()->subscribe<shared::protocol::server::Bct>(
            [this](const shared::protocol::server::Bct& cmd) { onBctReceived(cmd); }));
        getEventTokens().push_back(getDispatcher()->subscribe<shared::protocol::server::Pic>(
            [this](const shared::protocol::server::Pic& cmd) { onPicReceived(cmd); }));
        getEventTokens().push_back(getDispatcher()->subscribe<shared::protocol::server::Pie>(
            [this](const shared::protocol::server::Pie& cmd) { onPieReceived(cmd); }));
    }
}

TileInspectorUI::~TileInspectorUI() {
    if (getDispatcher() && getEventTokens().size() >= ExpectedEventTokens) {
        getDispatcher()->unsubscribe<events::TileClicked>(getEventTokens().at(0));
        getDispatcher()->unsubscribe<events::PlayerClicked>(getEventTokens().at(1));
        getDispatcher()->unsubscribe<shared::protocol::server::Bct>(getEventTokens().at(2));
        getDispatcher()->unsubscribe<shared::protocol::server::Pic>(getEventTokens().at(3));
        getDispatcher()->unsubscribe<shared::protocol::server::Pie>(getEventTokens().at(4));
    }
}

void TileInspectorUI::buildScannerPanel() {
    _posText = std::make_shared<components::UIText>("(0, 0)", getFont());
    _posText->setFontSize(static_cast<int>(InfoFontSize));
    _posText->setColor(raylib::Color::DarkGray());

    _scannerTitle = std::make_shared<components::UIText>("Incantation Scanner", getFont());
    _scannerTitle->setFontSize(static_cast<int>(HeaderFontSize));
    _scannerTitle->setColor(raylib::Color::Black());

    _scannerInfo = std::make_shared<components::UIText>("No ritual active", getFont());
    _scannerInfo->setFontSize(static_cast<int>(InfoFontSize));
    _scannerInfo->setColor(raylib::Color::DarkGray());

    _foodIcon = std::make_shared<components::UIImage>("assets/images/ui/food.png");
    _foodIcon->setSize(FoodIconSize, FoodIconSize);
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
        if (getInventoryTexts().size() >= TotalInventoryItems) {
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

void TileInspectorUI::onPicReceived(const shared::protocol::server::Pic& cmd) {
    if (cmd.x == _targetGridX && cmd.y == _targetGridY) {
        _scannerInfo->setText("Ritual active! Lvl " + std::to_string(cmd.level) +
                              "\nPlayers: " + std::to_string(cmd.playerIds.size()));
    }
}

void TileInspectorUI::onPieReceived(const shared::protocol::server::Pie& cmd) {
    if (cmd.x == _targetGridX && cmd.y == _targetGridY) {
        _scannerInfo->setText("No ritual active\nResult: " + std::to_string(cmd.incantationResult));
    }
}

void TileInspectorUI::requestTileSync() const {
    if (getOnSendCommand() && _targetGridX >= 0 && _targetGridY >= 0) {
        getOnSendCommand()(
            shared::protocol::Emitter::build(shared::protocol::client::Bct{.x = _targetGridX, .y = _targetGridY}));
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

void TileInspectorUI::drawTileInventoryPanel(float& currentY, float startX, float panelW) {
    float const titleX = startX + ((panelW - getInventoryTitleText()->getWidth()) / CenterDivisor);
    getInventoryTitleText()->setPosition(titleX, currentY);
    getInventoryTitleText()->draw();
    currentY += ElementSpacingSmall;

    if (getInventoryTexts().empty()) {
        return;
    }

    float const foodBoxX = startX + ((panelW - FoodBoxWidth) / CenterDivisor);
    raylib::rshapes::Shapes::drawRectangleRec(
        {.x = foodBoxX, .y = currentY, .width = FoodBoxWidth, .height = FoodBoxHeight}, BoxBgColor);
    _foodIcon->setPosition(foodBoxX + FoodIconOffset, currentY + FoodIconDrawOffset);
    _foodIcon->draw();

    auto& foodText = getInventoryTexts().at(0);
    foodText->setPosition(foodBoxX + FoodTextOffset, currentY + FoodIconOffset);
    foodText->draw();

    currentY += FoodBoxHeight + (ElementSpacingSmall - BoxMarginAdjustment);

    float const gridX = startX + ((panelW - GridTotalWidth) / CenterDivisor);
    for (size_t i = InventoryStartIndex; i < getInventoryTexts().size() && i <= InventoryEndIndex; ++i) {
        const size_t gridIndex = i - InventoryStartIndex;
        const int row = static_cast<int>(gridIndex) / GridColumns;
        const int col = static_cast<int>(gridIndex) % GridColumns;
        auto& invText = getInventoryTexts().at(i);
        float const cellX = gridX + (static_cast<float>(col) * GridCellSpacing);
        float const cellY = currentY + (static_cast<float>(row) * GridCellSpacing);

        raylib::rshapes::Shapes::drawRectangleRec(
            {.x = cellX, .y = cellY, .width = GridCellSize, .height = GridCellSize}, BoxBgColor);

        invText->setPosition(cellX + GridIconOffsetX, cellY + GridIconOffsetY);
        invText->draw();

        auto img = getInventoryImages().at(i);
        img->setPosition(cellX + GridIconOffsetX, cellY + GridIconOffsetY);
        img->draw();
    }
    currentY += GridCellSpacing * DoubleSpacingMultiplier;
}

void TileInspectorUI::draw() {
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

    if (_posText) {
        float const posTextW = _posText->getWidth();
        _posText->setPosition(startX + ((panelW - posTextW) / CenterDivisor), currentY);
        _posText->draw();
    }
    currentY += ElementSpacingMedium;

    float const scannerTitleW = _scannerTitle->getWidth();
    _scannerTitle->setPosition(startX + ((panelW - scannerTitleW) / CenterDivisor), currentY);
    _scannerTitle->draw();
    currentY += ElementSpacingSmall;

    float const scannerInfoW = _scannerInfo->getWidth();
    _scannerInfo->setPosition(startX + ((panelW - scannerInfoW) / CenterDivisor), currentY);
    _scannerInfo->draw();
    currentY += ElementSpacingLarge;

    drawTileInventoryPanel(currentY, startX, panelW);

    raylib::rcore::Window::endScissorMode();
}

}  // namespace zappy::gui::ui::menus
