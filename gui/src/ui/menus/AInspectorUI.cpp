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
#include <string_view>
#include <utility>

#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "components/UIButton.hpp"
#include "components/UIGamePanel.hpp"
#include "components/UIImage.hpp"
#include "components/UIText.hpp"
#include "graphics/AssetManager.hpp"
#include "protocol/Commands.hpp"
#include "rshapes/Shapes.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::menus {
AInspectorUI::AInspectorUI(float x, float y, float width, const std::string& title, events::EventDispatcher& dispatcher,
                           const std::shared_ptr<raylib::rtext::Font>& font,
                           std::function<void(const std::string&)> onSendCommand)
    : components::UIGamePanel(x, y, width, InspectorInitialHeight, title),
      _dispatcher(dispatcher),
      _font(font),
      _onSendCommand(std::move(onSendCommand)),
      _closeBtn(std::make_shared<components::UIButton>(x + width, y, CloseBtnSize, CloseBtnSize, "X", _font)) {
    setCustomLayout(true);
    setVisible(false);

    _closeBtn->setFontSize(CloseBtnFontSize);
    _closeBtn->setOnClick([this]() { this->setVisible(false); });
    addHeaderComponent(_closeBtn);

    _serverFrequencyToken = _dispatcher.get().subscribe<shared::protocol::server::Sgt>(
        [this](const shared::protocol::server::Sgt& cmd) { _serverFreq = static_cast<float>(cmd.timeUnit); });
}

AInspectorUI::~AInspectorUI() {
    if (_serverFrequencyToken != 0) {
        _dispatcher.get().unsubscribe<shared::protocol::server::Sgt>(_serverFrequencyToken);
    }
}

void AInspectorUI::buildInventoryPanel(bool includeFood) {
    _inventoryTitleText = std::make_shared<components::UIText>("Inventory", _font);
    _inventoryTitleText->setFontSize(static_cast<int>(HeaderFontSize));
    _inventoryTitleText->setColor(raylib::Color::Black());

    auto& assetManager = graphics::AssetManager::getInstance();
    for (const auto& asset : InventoryIconAssets) {
        if (!includeFood && asset.id == "inventory.food"sv) {
            continue;
        }
        auto text = std::make_shared<components::UIText>("0", _font);
        text->setFontSize(static_cast<int>(InfoFontSize));
        text->setColor(raylib::Color::White());
        _inventoryTexts.push_back(text);

        if (assetManager.loadTexture(asset.id, asset.path)) {
            auto img = std::make_shared<components::UIImage>(assetManager.getTexture(asset.id), asset.id);
            img->setSize(InvIconSize, InvIconSize);
            _inventoryImages.push_back(img);
        }
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

        auto img = _inventoryImages.at(i);
        img->setPosition(cellX + InvCellIconOffsetX, cellY + InvCellIconOffsetY);
        img->draw();
        invText->draw();
    }
    currentY += InvCellSpacing * 3.0F;
}

void AInspectorUI::draw() { components::UIGamePanel::draw(); }

void AInspectorUI::update() {
    components::UIGamePanel::update();
    if (_closeBtn) {
        _closeBtn->update();
    }
}

void AInspectorUI::handleEvent() {
    components::UIGamePanel::handleEvent();
    if (!isVisible()) {
        return;
    }
    if (_closeBtn) {
        _closeBtn->handleEvent();
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
