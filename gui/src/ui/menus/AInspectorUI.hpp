/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** AInspectorUI
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "events/EventDispatcher.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIGamePanel.hpp"
#include "ui/components/UIImage.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {

class AInspectorUI : public components::UIGamePanel {
  public:
    static constexpr auto FOOD_ICON_PATH = "assets/images/ui/Food.png";
    static constexpr auto DERAUMERE_ICON_PATH = "assets/images/ui/Deraumere.png";
    static constexpr auto SIBUR_ICON_PATH = "assets/images/ui/Sibur.png";
    static constexpr auto MENDIANE_ICON_PATH = "assets/images/ui/Mendiane.png";
    static constexpr auto PHIRAS_ICON_PATH = "assets/images/ui/Phiras.png";
    static constexpr auto THYSTAME_ICON_PATH = "assets/images/ui/Thystame.png";
    static constexpr auto LINEMATE_ICON_PATH = "assets/images/ui/Linemate.png";

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

    std::vector<std::string> _inventoryFilesPaths = {FOOD_ICON_PATH,    LINEMATE_ICON_PATH, DERAUMERE_ICON_PATH,
                                                     SIBUR_ICON_PATH,   MENDIANE_ICON_PATH, PHIRAS_ICON_PATH,
                                                     THYSTAME_ICON_PATH};

    float _serverFreq{100.0F};
};

}  // namespace zappy::gui::ui::menus
