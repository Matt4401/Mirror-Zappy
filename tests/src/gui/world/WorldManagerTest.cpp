/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** WorldManagerTest
*/

#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "events/EventDispatcher.hpp"
#include "graphics/scene/Tile3D.hpp"
#include "graphics/scene/WorldManager.hpp"
#include "gui/src/game/ItemBag.hpp"
#include "gui/src/game/Player.hpp"
#include "protocol/Commands.hpp"

namespace zappy::gui::graphics::scene {
namespace {
class WorldManagerTest : public testing::Test {
  protected:
    void createPlayer(int id = 42, int x = 1, int y = 1, std::string teamName = "blue") {
        dispatcher.dispatch(shared::protocol::server::Msz{.width = 4, .height = 4});
        dispatcher.dispatch(shared::protocol::server::Tna{.teamName = teamName});
        dispatcher.dispatch(shared::protocol::server::Pnw{
            .playerId = id,
            .x = x,
            .y = y,
            .orientation = 1,
            .level = 1,
            .teamName = std::move(teamName),
        });
    }

    [[nodiscard]] const game::Player& requirePlayer(int id) const {
        const auto player = world.playerById(id);
        if (!player.has_value()) {
            throw std::logic_error("expected player is missing");
        }
        return player->get();
    }

    [[nodiscard]] const Tile3D& requireTile(int x, int y) const {
        const auto tile = world.tileAt({.x = x, .y = y});
        if (!tile.has_value()) {
            throw std::logic_error("expected tile is missing");
        }
        return tile->get();
    }

    events::EventDispatcher dispatcher;  // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes,
                                         // misc-non-private-member-variables-in-classes)
    WorldManager world{dispatcher};      // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes,
                                         // misc-non-private-member-variables-in-classes)
};

TEST_F(WorldManagerTest, MszBuildsTheMap) {
    dispatcher.dispatch(shared::protocol::server::Msz{.width = 3, .height = 2});

    EXPECT_EQ(world.width(), 3);
    EXPECT_EQ(world.height(), 2);
    EXPECT_EQ(world.tiles().size(), 6);
    EXPECT_TRUE(world.tileAt({.x = 2, .y = 1}).has_value());
    EXPECT_FALSE(world.tileAt({.x = 3, .y = 1}).has_value());
    EXPECT_EQ(requireTile(2, 1).gridPosition(), (Tile3DPosition{.x = 2, .y = 1}));
}

TEST_F(WorldManagerTest, BctReplacesTileResources) {
    dispatcher.dispatch(shared::protocol::server::Msz{.width = 2, .height = 2});
    dispatcher.dispatch(shared::protocol::server::Bct{
        .x = 1,
        .y = 0,
        .food = 4,
        .linemate = 2,
        .deraumere = 0,
        .sibur = 0,
        .mendiane = 0,
        .phiras = 0,
        .thystame = 1,
    });

    const auto& items = requireTile(1, 0).itemBag().items();
    ASSERT_EQ(items.size(), 3);

    const auto food =
        std::ranges::find_if(items, [](const game::ItemBag::Item& item) { return item.object->name() == "Food"; });
    ASSERT_NE(food, items.end());
    EXPECT_EQ(food->quantity, 4);
}

TEST_F(WorldManagerTest, TnaAddsEachTeamOnlyOnce) {
    dispatcher.dispatch(shared::protocol::server::Tna{.teamName = "red"});
    dispatcher.dispatch(shared::protocol::server::Tna{.teamName = "red"});

    ASSERT_EQ(world.teams().size(), 1);
    EXPECT_EQ(world.teams().front().name(), "red");
}

TEST_F(WorldManagerTest, PnwAddsAPlayerToItsTeam) {
    dispatcher.dispatch(shared::protocol::server::Msz{.width = 4, .height = 4});
    dispatcher.dispatch(shared::protocol::server::Tna{.teamName = "blue"});
    dispatcher.dispatch(shared::protocol::server::Pnw{
        .playerId = 42,
        .x = 2,
        .y = 1,
        .orientation = 3,
        .level = 4,
        .teamName = "blue",
    });

    ASSERT_EQ(world.teams().size(), 1);
    ASSERT_EQ(world.teams().front().players().size(), 1);
    const auto& player = world.teams().front().players().front();
    EXPECT_EQ(player.id(), 42);
    EXPECT_EQ(player.level(), 4);
    EXPECT_EQ(player.orientation(), game::Player::cardinalPoint::SOUTH);
}

TEST_F(WorldManagerTest, PpoUpdatesPlayerPositionAndOrientation) {
    createPlayer();

    dispatcher.dispatch(shared::protocol::server::Ppo{.playerId = 42, .x = 3, .y = 2, .orientation = 4});

    const auto& player = requirePlayer(42);
    EXPECT_EQ(player.tilePosition(), (Tile3DPosition{.x = 3, .y = 2}));
    EXPECT_EQ(player.orientation(), game::Player::cardinalPoint::WEST);
}

TEST_F(WorldManagerTest, PlayerMovesTowardsItsTargetAndStopsOnIt) {
    static constexpr float TestDeltaTime = 0.01F;
    createPlayer();
    const auto start = requirePlayer(42).position();

    dispatcher.dispatch(shared::protocol::server::Ppo{.playerId = 42, .x = 3, .y = 2, .orientation = 4});

    const auto target = requirePlayer(42).futurePosition();
    EXPECT_TRUE(requirePlayer(42).moving());

    world.movePlayers(TestDeltaTime);
    EXPECT_GT(requirePlayer(42).position().distance(start), 0.0F);
    EXPECT_GT(requirePlayer(42).position().distance(target), 0.0F);

    for (int tick = 0; tick < 100 && requirePlayer(42).moving(); ++tick) {
        world.movePlayers(TestDeltaTime);
    }
    EXPECT_EQ(requirePlayer(42).position(), target);
    EXPECT_FALSE(requirePlayer(42).moving());
}

TEST_F(WorldManagerTest, PlayerLeavesTheMapBeforeWrappingToTheOppositeEdge) {
    static constexpr float TestDeltaTime = 0.01F;
    createPlayer(42, 0, 1);
    const auto start = requirePlayer(42).position();

    dispatcher.dispatch(shared::protocol::server::Ppo{.playerId = 42, .x = 3, .y = 1, .orientation = 4});

    const auto wrappedPosition = requireTile(3, 1).position();
    world.movePlayers(TestDeltaTime);
    EXPECT_LT(requirePlayer(42).position().x(), start.x());
    EXPECT_TRUE(requirePlayer(42).moving());

    for (int tick = 0; tick < 100 && requirePlayer(42).moving(); ++tick) {
        world.movePlayers(TestDeltaTime);
    }
    EXPECT_FLOAT_EQ(requirePlayer(42).position().x(), wrappedPosition.x());
    EXPECT_FLOAT_EQ(requirePlayer(42).position().z(), wrappedPosition.z());
    EXPECT_FALSE(requirePlayer(42).moving());
}

TEST_F(WorldManagerTest, PlayerNameCanBeUpdatedByGuiEvent) {
    createPlayer();

    world.updatePlayerName(42, "Eliott");

    EXPECT_EQ(requirePlayer(42).name(), "Eliott");
}

TEST_F(WorldManagerTest, PlvUpdatesPlayerLevel) {
    createPlayer();

    dispatcher.dispatch(shared::protocol::server::Plv{.playerId = 42, .level = 6});

    EXPECT_EQ(requirePlayer(42).level(), 6);
}

TEST_F(WorldManagerTest, PinReplacesPlayerInventoryAndSynchronizesPosition) {
    createPlayer();

    dispatcher.dispatch(shared::protocol::server::Pin{
        .playerId = 42,
        .x = 2,
        .y = 3,
        .food = 8,
        .linemate = 1,
        .deraumere = 0,
        .sibur = 2,
        .mendiane = 0,
        .phiras = 0,
        .thystame = 0,
    });

    const auto& player = requirePlayer(42);
    EXPECT_EQ(player.tilePosition(), (Tile3DPosition{.x = 2, .y = 3}));
    EXPECT_EQ(player.itemBag().quantity("Food"), 8);
    EXPECT_EQ(player.itemBag().quantity("Linemate"), 1);
    EXPECT_EQ(player.itemBag().quantity("Sibur"), 2);
}

TEST_F(WorldManagerTest, PicAndPieTrackIncantationAndUpgradeParticipantsOnSuccess) {
    createPlayer(42, 1, 1);
    createPlayer(43, 1, 1);

    dispatcher.dispatch(shared::protocol::server::Pic{
        .x = 1,
        .y = 1,
        .level = 1,
        .playerIds = {42, 43},
    });

    ASSERT_EQ(world.activeIncantations().size(), 1);
    EXPECT_EQ(world.activeIncantations().front().playerIds.size(), 2);

    dispatcher.dispatch(shared::protocol::server::Pie{.x = 1, .y = 1, .incantationResult = true});

    EXPECT_TRUE(world.activeIncantations().empty());
    EXPECT_EQ(requirePlayer(42).level(), 2);
    EXPECT_EQ(requirePlayer(43).level(), 2);
}

TEST_F(WorldManagerTest, FailedPieClearsIncantationWithoutChangingLevel) {
    createPlayer();
    dispatcher.dispatch(shared::protocol::server::Pic{.x = 1, .y = 1, .level = 1, .playerIds = {42}});

    dispatcher.dispatch(shared::protocol::server::Pie{.x = 1, .y = 1, .incantationResult = false});

    EXPECT_TRUE(world.activeIncantations().empty());
    EXPECT_EQ(requirePlayer(42).level(), 1);
}

TEST_F(WorldManagerTest, PgtAndPdrTransferResourcesBetweenTileAndPlayer) {
    createPlayer();
    dispatcher.dispatch(shared::protocol::server::Bct{.x = 1, .y = 1, .food = 2});
    dispatcher.dispatch(shared::protocol::server::Pin{.playerId = 42, .x = 1, .y = 1, .food = 1});

    dispatcher.dispatch(shared::protocol::server::Pgt{.playerId = 42, .resourceId = 0});

    EXPECT_EQ(requirePlayer(42).itemBag().quantity("Food"), 2);
    EXPECT_EQ(requireTile(1, 1).itemBag().quantity("Food"), 1);

    dispatcher.dispatch(shared::protocol::server::Pdr{.playerId = 42, .resourceId = 0});

    EXPECT_EQ(requirePlayer(42).itemBag().quantity("Food"), 1);
    EXPECT_EQ(requireTile(1, 1).itemBag().quantity("Food"), 2);
}

TEST_F(WorldManagerTest, PdiRemovesPlayerFromWorld) {
    createPlayer();

    dispatcher.dispatch(shared::protocol::server::Pdi{.playerId = 42});

    EXPECT_FALSE(world.playerById(42).has_value());
    ASSERT_EQ(world.teams().size(), 1);
    EXPECT_TRUE(world.teams().front().players().empty());
}

TEST_F(WorldManagerTest, EnwAddsEggAndEboOrEdiRemoveIt) {
    createPlayer();

    dispatcher.dispatch(shared::protocol::server::Enw{.eggId = 7, .playerId = 42, .x = 1, .y = 2});
    dispatcher.dispatch(shared::protocol::server::Enw{.eggId = 8, .playerId = 42, .x = 2, .y = 2});

    ASSERT_EQ(world.teams().front().eggs().size(), 2);
    EXPECT_EQ(world.teams().front().eggs().front().id(), 7);

    dispatcher.dispatch(shared::protocol::server::Ebo{.eggId = 7});
    ASSERT_EQ(world.teams().front().eggs().size(), 1);
    EXPECT_EQ(world.teams().front().eggs().front().id(), 8);

    dispatcher.dispatch(shared::protocol::server::Edi{.eggId = 8});
    EXPECT_TRUE(world.teams().front().eggs().empty());
}

TEST_F(WorldManagerTest, GlobalWorldStateTracksTimeWinnerAndServerMessage) {
    dispatcher.dispatch(shared::protocol::server::Sgt{.timeUnit = 50});
    EXPECT_EQ(world.timeUnit(), 50);

    dispatcher.dispatch(shared::protocol::server::Sst{.timeUnit = 120});
    dispatcher.dispatch(shared::protocol::server::Seg{.teamName = "blue"});
    dispatcher.dispatch(shared::protocol::server::Smg{.message = "game over"});

    EXPECT_EQ(world.timeUnit(), 120);
    EXPECT_EQ(world.winningTeam(), std::optional<std::string>{"blue"});
    EXPECT_EQ(world.lastServerMessage(), "game over");
}
}  // namespace
}  // namespace zappy::gui::graphics::scene
