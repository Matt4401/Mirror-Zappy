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

#include "events/EventDispatcher.hpp"
#include "game/Player.hpp"
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

class PlayerInspectorUI : public components::UIGamePanel {
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

    void setTargetPlayer(int playerId, const game::Player& initialData);
    void setVisible(bool visible) override;
    void setPosition(float x, float y) override;

  private:
    void buildInfoPanel();
    void buildInventoryPanel();
    void updateHearts();
    void requestPlayerSync() const;

    void onPinReceived(const shared::protocol::server::Pin& cmd);
    void onPlvReceived(const shared::protocol::server::Plv& cmd);
    void onPpoReceived(const shared::protocol::server::Ppo& cmd);
    void onSgtReceived(const shared::protocol::server::Sgt& cmd);

    std::shared_ptr<events::EventDispatcher> _dispatcher;
    std::shared_ptr<raylib::rtext::Font> _font;
    std::function<void(const std::string&)> _onSendCommand;
    std::vector<events::EventDispatcher::EventToken> _eventTokens;
    int _targetPlayerId{-1};

    std::shared_ptr<components::UIText> _inventoryTitleText;

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

    std::vector<std::shared_ptr<components::UIImage>> _inventoryImages;
    std::vector<std::shared_ptr<components::UIText>> _inventoryTexts;
    std::shared_ptr<components::UIButton> _firstPersonBtn;
    std::shared_ptr<components::UIButton> _closeBtn;

    raylib::rcore::Camera _avatarCamera;
    std::shared_ptr<raylib::rmodels::Model> _avatarModel;
    std::shared_ptr<raylib::rtextures::RenderTexture2D> _avatarRenderTexture;

    float _foodFloat{10.0F};
    float _serverFreq{100.0F};
    int _levelValue{1};
    int _orientationValue{1};
};

}  // namespace zappy::gui::ui::menus
