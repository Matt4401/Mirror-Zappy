/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** WorldControlUI
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "events/EventDispatcher.hpp"
#include "rcore/Event.hpp"
#include "rtext/Font.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIDropdown.hpp"
#include "ui/components/UIGamePanel.hpp"
#include "ui/components/UIPanel.hpp"
#include "ui/components/UISlider.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {

class WorldControlUI : public components::UIGamePanel {
  public:
    WorldControlUI(float x, float width, std::shared_ptr<events::EventDispatcher> dispatcher,
                   std::shared_ptr<raylib::rtext::Font> font, std::function<void(const std::string&)> onSendCommand);
    ~WorldControlUI() override;

    WorldControlUI(const WorldControlUI& other) = delete;
    WorldControlUI& operator=(const WorldControlUI& other) = delete;
    WorldControlUI(WorldControlUI&& other) = delete;
    WorldControlUI& operator=(WorldControlUI&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent(const raylib::rcore::Event& event) override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;

    [[nodiscard]] bool isHovered() const override;

  private:
    void initSpeedControls();
    void initTimeControls();
    void initEventSubscriptions();

    void updateChildrenPositions();

    std::shared_ptr<events::EventDispatcher> _dispatcher;
    std::shared_ptr<raylib::rtext::Font> _font;
    std::function<void(const std::string&)> _onSendCommand;
    std::vector<events::EventDispatcher::EventToken> _eventTokens;

    bool _isDragging{false};

    bool _isFolded{true};
    float _currentHeight;

    std::shared_ptr<components::UIPanel> _contentPanel;
    std::shared_ptr<components::UIButton> _foldBtn;
    std::shared_ptr<components::UISlider> _speedSlider;
    std::shared_ptr<components::UIDropdown> _timeDropdown;
    std::shared_ptr<components::UIText> _speedTitle;
    std::shared_ptr<components::UIText> _timeTitle;
    std::shared_ptr<components::UIText> _speedValueText;

    float _serverFreq{100.0F};

    static constexpr float FoldBtnWidth = 80.0F;
    static constexpr float FoldBtnHeight = 35.0F;
    static constexpr float ContentHeight = 60.0F;
    static constexpr float LerpSpeedMultiplier = 15.0F;

    static constexpr int DefaultFontSize = 16;
    static constexpr float DropdownHeight = 24.0F;
    static constexpr float DropdownWidth = 150.0F;

    static constexpr float SliderWidth = 200.0F;
    static constexpr float SliderHeight = 20.0F;
    static constexpr float SliderMin = 2.0F;
    static constexpr float SliderMax = 1000.0F;
    static constexpr float SliderDefault = 100.0F;

    static constexpr float InitialOffsetX = 130.0F;
    static constexpr float TitleOffsetY = 4.0F;
    static constexpr float SliderOffsetY = 2.0F;

    static constexpr float TimeTitleWidth = 130.0F;
    static constexpr float TimeDropdownWidth = 170.0F;
    static constexpr float SpeedTitleWidth = 190.0F;
    static constexpr float SpeedSliderWidth = 210.0F;

    static constexpr float FoldAnimThreshold = 0.5F;
    static constexpr float MinDrawHeight = 0.5F;
    static constexpr float MinUpdateHeight = 5.0F;
};

}  // namespace zappy::gui::ui::menus
