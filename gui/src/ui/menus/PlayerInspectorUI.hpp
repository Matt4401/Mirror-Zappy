/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** PlayerInspectorUI
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "AInspectorUI.hpp"
#include "events/EventDispatcher.hpp"
#include "events/GuiEvents.hpp"
#include "protocol/Commands.hpp"
#include "rcore/Camera.hpp"
#include "rcore/Event.hpp"
#include "rmath/Vector3.hpp"
#include "rmodels/Model.hpp"
#include "rtext/Font.hpp"
#include "rtextures/RenderTexture2D.hpp"
#include "ui/components/UIButton.hpp"
#include "ui/components/UIImage.hpp"
#include "ui/components/UIText.hpp"

namespace zappy::gui::ui::menus {

class PlayerInspectorUI : public AInspectorUI {
  public:
    PlayerInspectorUI(float x, float y, float width, std::shared_ptr<events::EventDispatcher> dispatcher,
                      const std::shared_ptr<raylib::rtext::Font>& font,
                      std::function<void(const std::string&)> onSendCommand = nullptr);
    ~PlayerInspectorUI() override;

    PlayerInspectorUI(const PlayerInspectorUI& other) = delete;
    PlayerInspectorUI& operator=(const PlayerInspectorUI& other) = delete;
    PlayerInspectorUI(PlayerInspectorUI&& other) = delete;
    PlayerInspectorUI& operator=(PlayerInspectorUI&& other) = delete;

    void draw() override;
    void update() override;
    void handleEvent(const raylib::rcore::Event& event) override;

    void onPlayerClicked(const events::PlayerClicked& event);
    void setVisible(bool visible) override;
    void setPosition(float x, float y) override;

  private:
    void buildInfoPanel();
    void updateHearts();
    void requestPlayerSync() const;

    void drawHeader(float& currentY, float startX, float panelW);
    void drawStatsBlock(float& currentY, float startX, float panelW);
    void drawXpBar(float& currentY, float startX, float panelW);
    void drawHealthBar(float& currentY, float startX, float panelW);
    void drawActionButtons(float& currentY, float startX, float panelW);

    void onPinReceived(const shared::protocol::server::Pin& cmd);
    void onPlvReceived(const shared::protocol::server::Plv& cmd);
    void onPpoReceived(const shared::protocol::server::Ppo& cmd);

    int _targetPlayerId{-1};

    std::shared_ptr<components::UIText> _nameText;
    std::shared_ptr<components::UIText> _idText;
    std::shared_ptr<components::UIText> _teamText;
    std::shared_ptr<components::UIText> _posText;
    std::shared_ptr<components::UIText> _compassText;

    std::vector<std::shared_ptr<components::UIImage>> _baseHearts;
    std::vector<std::shared_ptr<components::UIImage>> _overlapHearts;
    std::vector<std::shared_ptr<components::UIImage>> _xpBar;

    std::shared_ptr<components::UIImage> _teamIcon;
    std::shared_ptr<components::UIImage> _idIcon;
    std::shared_ptr<components::UIImage> _posIcon;
    std::shared_ptr<components::UIImage> _levelIcon;
    std::shared_ptr<components::UIImage> _compassIcon;

    std::shared_ptr<components::UIButton> _firstPersonBtn;

    float _foodFloat{10.0F};
    int _levelValue{1};
    int _orientationValue{1};

    void draw3DPreview();

    raylib::rcore::Camera _previewCamera;
    std::shared_ptr<raylib::rmodels::Model> _previewModel;
    std::shared_ptr<raylib::rtextures::RenderTexture2D> _previewRenderTexture;
    float _previewModelScale{0.1F};
    raylib::rmath::Vector3 _previewModelOffset{0.0F, 0.0F, 0.0F};
};

}  // namespace zappy::gui::ui::menus
