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
#include "components/UIText.hpp"
#include "events/GuiEvents.hpp"
#include "protocol/Commands.hpp"
#include "rtext/Font.hpp"

namespace zappy::gui::ui::menus {

class TileInspectorUI : public AInspectorUI {
  public:
    TileInspectorUI(float x, float y, float width, std::shared_ptr<events::EventDispatcher> dispatcher,
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

  private:
    void buildScannerPanel();
    void requestTileSync() const;

    void onBctReceived(const shared::protocol::server::Bct& cmd);

    int _targetGridX{-1};
    int _targetGridY{-1};

    std::shared_ptr<components::UIText> _titleText;
    std::shared_ptr<components::UIText> _posText;
    std::shared_ptr<components::UIText> _scannerTitle;
    std::shared_ptr<components::UIText> _scannerInfo;
};

}  // namespace zappy::gui::ui::menus
