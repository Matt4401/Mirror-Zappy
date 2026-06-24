/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** EventLogRow
*/

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::components {

class EventLogRow : public IUIComponent {
  public:
    EventLogRow(const std::vector<std::pair<std::string, raylib::Color>>& segments,
                const std::shared_ptr<raylib::rtext::Font>& font);
    ~EventLogRow() override = default;

    EventLogRow(const EventLogRow& other) = delete;
    EventLogRow& operator=(const EventLogRow& other) = delete;
    EventLogRow(EventLogRow&& other) = delete;
    EventLogRow& operator=(EventLogRow&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override;

  private:
    std::vector<std::shared_ptr<components::UIText>> _texts;
    raylib::rmath::Vector2 _position{0.0F, 0.0F};
    raylib::rmath::Vector2 _size{0.0F, 0.0F};
    bool _isVisible{true};

    static constexpr float FontSize = 16.0F;
};

}  // namespace zappy::gui::ui::components
