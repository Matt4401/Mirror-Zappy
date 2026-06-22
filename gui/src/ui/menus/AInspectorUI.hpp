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
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rmodels/Model.hpp"
#include "rtext/Font.hpp"
#include "rtextures/RenderTexture2D.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIGamePanel.hpp"
#include "ui/components/UIImage.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {

class AInspectorUI : public components::UIGamePanel {
  public:
    AInspectorUI(float x, float y, float width, const std::string& title,
                 std::shared_ptr<events::EventDispatcher> dispatcher, const std::shared_ptr<raylib::rtext::Font>& font,
                 std::function<void(const std::string&)> onSendCommand);
    ~AInspectorUI() override;

    AInspectorUI(const AInspectorUI& other) = delete;
    AInspectorUI& operator=(const AInspectorUI& other) = delete;
    AInspectorUI(AInspectorUI&& other) = delete;
    AInspectorUI& operator=(AInspectorUI&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent(const raylib::rcore::Event& event) override;

    void setVisible(bool visible) override;
    void setPosition(float x, float y) override;

  protected:
    void buildInventoryPanel();
    void drawInventoryPanel(float& currentY, float startX, float panelW);
    void draw3DPreview();

    [[nodiscard]] std::shared_ptr<events::EventDispatcher>& getDispatcher() { return _dispatcher; }
    [[nodiscard]] std::shared_ptr<raylib::rtext::Font>& getFont() { return _font; }
    [[nodiscard]] const std::function<void(const std::string&)>& getOnSendCommand() const { return _onSendCommand; }
    [[nodiscard]] std::vector<events::EventDispatcher::EventToken>& getEventTokens() { return _eventTokens; }

    [[nodiscard]] std::shared_ptr<components::UIText>& getInventoryTitleText() { return _inventoryTitleText; }
    [[nodiscard]] std::vector<std::shared_ptr<components::UIImage>>& getInventoryImages() { return _inventoryImages; }
    [[nodiscard]] std::vector<std::shared_ptr<components::UIText>>& getInventoryTexts() { return _inventoryTexts; }

    [[nodiscard]] raylib::rcore::Camera& getPreviewCamera() { return _previewCamera; }
    [[nodiscard]] std::shared_ptr<raylib::rmodels::Model>& getPreviewModel() { return _previewModel; }
    [[nodiscard]] std::shared_ptr<raylib::rtextures::RenderTexture2D>& getPreviewRenderTexture() {
        return _previewRenderTexture;
    }
    [[nodiscard]] float getServerFreq() const { return _serverFreq; }

  private:
    std::shared_ptr<events::EventDispatcher> _dispatcher;
    std::shared_ptr<raylib::rtext::Font> _font;
    std::function<void(const std::string&)> _onSendCommand;
    std::vector<events::EventDispatcher::EventToken> _eventTokens;

    std::shared_ptr<components::UIText> _inventoryTitleText;
    std::vector<std::shared_ptr<components::UIImage>> _inventoryImages;
    std::vector<std::shared_ptr<components::UIText>> _inventoryTexts;
    std::shared_ptr<components::UIButton> _closeBtn;

    raylib::rcore::Camera _previewCamera;
    std::shared_ptr<raylib::rmodels::Model> _previewModel;
    std::shared_ptr<raylib::rtextures::RenderTexture2D> _previewRenderTexture;
    float _serverFreq{100.0F};
};

}  // namespace zappy::gui::ui::menus
