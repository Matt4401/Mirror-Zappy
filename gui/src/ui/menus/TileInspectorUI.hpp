/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** TileInspectorUI
*/

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "AInspectorUI.hpp"
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

    graphics::scene::Tile3DPosition _targetGridPosition{-1, -1};

    std::shared_ptr<components::UIText> _posText;
    std::shared_ptr<components::UIText> _scannerTitle;
    std::shared_ptr<components::UIText> _scannerInfo;
    std::shared_ptr<components::UIImage> _foodIcon;
};

}  // namespace zappy::gui::ui::menus
