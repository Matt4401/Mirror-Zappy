/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** AInspectorUI
*/

#pragma once

#include <array>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "events/EventDispatcher.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIGamePanel.hpp"
#include "ui/components/UIImage.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {
using std::literals::string_view_literals::operator""sv;

class AInspectorUI : public components::UIGamePanel {
    static constexpr float InspectorInitialHeight = 550.0F;

    static constexpr float InvGridTotalWidth = 230.0F;
    static constexpr float InvTitleToGridSpacing = 30.0F;
    static constexpr float InvCellSize = 70.0F;
    static constexpr float InvCellSpacing = 80.0F;
    static constexpr float InvCellTextOffsetX = 10.0F;
    static constexpr float InvCellTextOffsetY = 50.0F;
    static constexpr float InvCellIconOffsetX = 18.0F;
    static constexpr float InvCellIconOffsetY = 13.0F;

  protected:
    static constexpr float InfoFontSize = 16.0F;
    static constexpr float HeaderFontSize = 20.0F;

  private:
    static constexpr float CloseBtnSize = 24.0F;
    static constexpr float CloseBtnFontSize = 16.0F;
    static constexpr float CloseBtnXOffset = 45.0F;
    static constexpr float CloseBtnYOffset = 8.0F;

    static constexpr float InvIconSize = 40.0F;

    static constexpr int ElementCount = 7;

    struct InventoryIconAsset {
        std::string_view id;
        std::string_view path;
    };

    static constexpr std::array<InventoryIconAsset, ElementCount> InventoryIconAssets = {{
        ("inventory.food"sv, "assets/images/ui/Food.png"sv),
        ("inventory.linemate"sv, "assets/images/ui/Linemate.png"sv),
        ("inventory.deraumere"sv, "assets/images/ui/Deraumere.png"sv),
        ("inventory.sibur"sv, "assets/images/ui/Sibur.png"sv),
        ("inventory.mendiane"sv, "assets/images/ui/Mendiane.png"sv),
        ("inventory.phiras"sv, "assets/images/ui/Phiras.png"sv),
        ("inventory.thystame"sv, "assets/images/ui/Thystame.png"sv),
    }};

  public:
    AInspectorUI(float x, float y, float width, const std::string& title, events::EventDispatcher& dispatcher,
                 const std::shared_ptr<raylib::rtext::Font>& font,
                 std::function<void(const std::string&)> onSendCommand);
    ~AInspectorUI() override = 0;

    AInspectorUI(const AInspectorUI& other) = delete;
    AInspectorUI& operator=(const AInspectorUI& other) = delete;
    AInspectorUI(AInspectorUI&& other) = delete;
    AInspectorUI& operator=(AInspectorUI&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setVisible(bool visible) override;
    void setPosition(float x, float y) override;

  protected:
    void buildInventoryPanel();
    void drawInventoryPanel(float& currentY, float startX, float panelW);

    [[nodiscard]] events::EventDispatcher& getDispatcher() { return _dispatcher.get(); }
    [[nodiscard]] std::shared_ptr<raylib::rtext::Font>& getFont() { return _font; }
    [[nodiscard]] const std::function<void(const std::string&)>& getOnSendCommand() const { return _onSendCommand; }
    [[nodiscard]] std::vector<events::EventDispatcher::EventToken>& getEventTokens() { return _eventTokens; }

    [[nodiscard]] std::shared_ptr<components::UIText>& getInventoryTitleText() { return _inventoryTitleText; }
    [[nodiscard]] std::vector<std::shared_ptr<components::UIImage>>& getInventoryImages() { return _inventoryImages; }
    [[nodiscard]] std::vector<std::shared_ptr<components::UIText>>& getInventoryTexts() { return _inventoryTexts; }

    [[nodiscard]] float getServerFreq() const { return _serverFreq; }

  private:
    std::reference_wrapper<events::EventDispatcher> _dispatcher;
    std::shared_ptr<raylib::rtext::Font> _font;
    std::function<void(const std::string&)> _onSendCommand;
    events::EventDispatcher::EventToken _serverFrequencyToken{0};
    std::vector<events::EventDispatcher::EventToken> _eventTokens;

    std::shared_ptr<components::UIText> _inventoryTitleText;
    std::vector<std::shared_ptr<components::UIImage>> _inventoryImages;
    std::vector<std::shared_ptr<components::UIText>> _inventoryTexts;
    std::shared_ptr<components::UIButton> _closeBtn;

    float _serverFreq{100.0F};
};

}  // namespace zappy::gui::ui::menus
