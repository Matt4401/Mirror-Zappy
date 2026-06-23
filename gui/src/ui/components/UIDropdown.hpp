/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** UIDropdown
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Color.hpp"
#include "rmath/Vector2.hpp"
#include "rtext/Font.hpp"
#include "ui/IUIComponent.hpp"

namespace zappy::gui::ui::components {

class UIDropdown : public IUIComponent {
  public:
    enum class Direction : std::uint8_t { DOWN, UP };

    UIDropdown(float x, float y, float width, float height, std::vector<std::string> options,
               std::shared_ptr<raylib::rtext::Font> font);
    ~UIDropdown() override = default;

    UIDropdown(const UIDropdown& other) = delete;
    UIDropdown& operator=(const UIDropdown& other) = delete;
    UIDropdown(UIDropdown&& other) = delete;
    UIDropdown& operator=(UIDropdown&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent() override;

    void setPosition(float x, float y) override;
    void setSize(float width, float height) override;
    [[nodiscard]] bool isVisible() const override;
    void setVisible(bool visible) override;
    [[nodiscard]] bool isHovered() const override { return _isMainHovered || _isOpen; }

    void setOnSelect(std::function<void(const std::string&)> callback);
    void setDirection(Direction dir);
    void setSelectedIndex(size_t index);
    [[nodiscard]] size_t getSelectedIndex() const;
    [[nodiscard]] std::string getSelectedOption() const;
    [[nodiscard]] bool isOpen() const { return _isOpen; }
    [[nodiscard]] size_t getOptionsCount() const { return _options.size(); }
    [[nodiscard]] raylib::rmath::Vector2 getPosition() const { return _position; }
    [[nodiscard]] raylib::rmath::Vector2 getSize() const { return _size; }

  private:
    void updateTextPositions();

    raylib::rmath::Vector2 _position;
    raylib::rmath::Vector2 _size;
    bool _isVisible{true};

    std::vector<std::string> _options;
    size_t _selectedIndex{0};
    bool _isOpen{false};
    Direction _direction{Direction::UP};

    std::shared_ptr<raylib::rtext::Font> _fontRef;
    std::function<void(const std::string&)> _onSelect;

    std::optional<size_t> _hoveredIndex{std::nullopt};
    bool _isMainHovered{false};

    static constexpr int MouseLeftButton = 0;
    static constexpr float DefaultFontSize = 16.0F;
    static constexpr float TextPaddingX = 8.0F;

    static const raylib::Color BackgroundColor;
    static const raylib::Color HoverColor;
    static const raylib::Color BorderColor;
    static const raylib::Color TextColor;
};

}  // namespace zappy::gui::ui::components
