/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** TileInspectorUI
*/

#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

#include "AInspectorUI.hpp"
#include "Color.hpp"
#include "EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "protocol/Commands.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIImage.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {
class TileInspectorUI : public AInspectorUI {
  public:
    static constexpr float HeaderFontSize = 20.0F;
    static constexpr float InfoFontSize = 16.0F;
    static constexpr float BaseYOffset = 60.0F;
    static constexpr float ElementSpacingSmall = 30.0F;
    static constexpr float ElementSpacingMedium = 40.0F;
    static constexpr float ElementSpacingLarge = 60.0F;

    static constexpr float FoodBoxWidth = 100.0F;
    static constexpr float FoodBoxHeight = 40.0F;
    static constexpr float FoodIconOffset = 10.0F;
    static constexpr float FoodIconSize = 30.0F;
    static constexpr float FoodTextOffset = 50.0F;

    static constexpr float GridCellSize = 70.0F;
    static constexpr float GridCellSpacing = 80.0F;
    static constexpr float GridTotalWidth = 230.0F;
    static constexpr float GridIconOffsetX = 18.0F;
    static constexpr float GridIconOffsetY = 13.0F;
    static constexpr float GridTextOffsetX = 10.0F;
    static constexpr float GridTextOffsetY = 50.0F;

    static constexpr float FoodIconDrawOffset = 5.0F;
    static constexpr float BoxMarginAdjustment = 10.0F;
    static constexpr float DoubleSpacingMultiplier = 2.0F;

    static constexpr size_t InventoryStartIndex = 1;
    static constexpr size_t InventoryEndIndex = 6;
    static constexpr int GridColumns = 3;
    static constexpr size_t TotalInventoryItems = 7;
    static constexpr size_t ExpectedEventTokens = 5;

    static constexpr float CenterDivisor = 2.0F;

    static constexpr raylib::Color BoxBgColor{200, 200, 200, 255};

    TileInspectorUI(float x, float y, float width, events::EventDispatcher& dispatcher,
                    const std::shared_ptr<raylib::rtext::Font>& font,
                    std::function<void(const std::string&)> onSendCommand = nullptr);
    ~TileInspectorUI() override;

    TileInspectorUI(const TileInspectorUI& other) = delete;
    TileInspectorUI& operator=(const TileInspectorUI& other) = delete;
    TileInspectorUI(TileInspectorUI&& other) = delete;
    TileInspectorUI& operator=(TileInspectorUI&& other) = delete;

    void draw() override;
    void update() override;

    void onTileClicked(const events::TileClicked& event);
    void setVisible(bool visible) override;

    void onPicReceived(const shared::protocol::server::Pic& cmd);
    void onPieReceived(const shared::protocol::server::Pie& cmd);

  private:
    void buildScannerPanel();
    void requestTileSync() const;
    void drawTileInventoryPanel(float& currentY, float startX, float panelW);

    void onBctReceived(const shared::protocol::server::Bct& cmd);

    graphics::scene::Tile3DPosition _targetGridPosition = graphics::scene::Tile3DPosition(-1, -1);

    std::shared_ptr<components::UIText> _posText;
    std::shared_ptr<components::UIText> _scannerTitle;
    std::shared_ptr<components::UIText> _scannerInfo;
    std::shared_ptr<components::UIImage> _foodIcon;
};

}  // namespace zappy::gui::ui::menus
