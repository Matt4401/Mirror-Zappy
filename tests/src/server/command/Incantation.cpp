#include "command/Incantation.hpp"

#include <gtest/gtest.h>

#include <memory>

#include "game/Player.hpp"
#include "game/World.hpp"

namespace zappy::server::command {

class IncantationTest : public ::testing::Test {
  protected:
    // Configuration de base pour générer un World de test
    parser::ServerConfig config{
        .port = 4242,
        .width = 10,
        .height = 10,
        .teamNames = {"Team1"},
        .clientLimit = 10,
        .freq = 100,
    };
};

// 1. Test du nombre de ticks requis
TEST_F(IncantationTest, CheckRequiredTicks) {
    const std::unique_ptr<ICommand> incantation = std::make_unique<Incantation>();
    ASSERT_EQ(incantation->requiredTicks(), 300);
}

// 2. Test du refus de l'incantation si les ressources manquent
TEST_F(IncantationTest, StartFailsWithoutResources) {
    game::World world{config};
    auto playerIdOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(playerIdOpt.has_value());

    world.clearAllResourcesAndEggs();
    auto& player = *world.playerList().at(playerIdOpt.value());

    const std::unique_ptr<ICommand> incantation = std::make_unique<Incantation>();

    // Le niveau 1 demande 1 linemate. Sans ressource, start() doit renvoyer false.
    ASSERT_FALSE(incantation->start(world, player));
}

// 3. Test de validation du Start (ressources présentes) et émission de l'event GUI (pic)
TEST_F(IncantationTest, StartSucceedsWithResources) {
    game::World world{config};
    auto playerIdOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(playerIdOpt.has_value());

    world.clearAllResourcesAndEggs();
    auto& player = *world.playerList().at(playerIdOpt.value());

    // Ajout de la pierre requise pour le niveau 1 -> 2
    world.addItemOnGround(game::ItemType::Linemate, player.position());

    const std::unique_ptr<ICommand> incantation = std::make_unique<Incantation>();

    ASSERT_TRUE(incantation->start(world, player));

    // Vérification de la transmission de l'event graphique (pic)
    auto guiEvents = world.getAndClearGuiEvents();
    ASSERT_FALSE(guiEvents.empty());
    EXPECT_NE(guiEvents.back().find("pic"), std::string::npos);
}

// 4. Test de l'exécution finale (Niveau 1 -> 2) : Réponse, LevelUp et Consommation des Stones
TEST_F(IncantationTest, ExecuteSuccessAndReplies) {
    game::World world{config};
    auto playerIdOpt = world.spawnPlayer("Team1");
    ASSERT_TRUE(playerIdOpt.has_value());

    world.clearAllResourcesAndEggs();
    auto& player = *world.playerList().at(playerIdOpt.value());
    const auto playerPos = player.position();

    // On s'assure que le joueur démarre bien niveau 1
    ASSERT_EQ(player.level(), 1);

    // Ajout de la ressource requise au sol
    world.addItemOnGround(game::ItemType::Linemate, playerPos);
    ASSERT_EQ(world.resourcesAt(playerPos).at(static_cast<std::uint8_t>(game::ItemType::Linemate)), 1);

    const std::unique_ptr<ICommand> incantation = std::make_unique<Incantation>();

    // Exécution de l'incantation
    incantation->execute(world, player);

    // 1. Vérification des réponses du joueur et de l'event GUI (pie)
    auto responses = player.responses();
    ASSERT_FALSE(responses.empty());
    EXPECT_EQ(responses.front(), "ok\n");

    auto guiEvents = world.getAndClearGuiEvents();
    ASSERT_FALSE(guiEvents.empty());
    EXPECT_NE(guiEvents.back().find("pie"), std::string::npos);

    // 2. CHECK : Vérification du LevelUp du joueur initiateur
    EXPECT_EQ(player.level(), 2);

    // 3. CHECK : Vérification de la consommation des pierres sur la case
    auto resourcesAfter = world.resourcesAt(playerPos);
    EXPECT_EQ(resourcesAfter.at(static_cast<std::uint8_t>(game::ItemType::Linemate)), 0);
}

// 5. Test complexe (Niveau 2 -> 3) : Vérification du LevelUp de CHAQUE joueur présent
TEST_F(IncantationTest, MultiPlayerRequirementLevel2To3) {
    game::World world{config};
    auto id1Opt = world.spawnPlayer("Team1");
    auto id2Opt = world.spawnPlayer("Team1");
    ASSERT_TRUE(id1Opt.has_value() && id2Opt.has_value());

    world.clearAllResourcesAndEggs();

    auto& p1 = *world.playerList().at(id1Opt.value());
    auto& p2 = *world.playerList().at(id2Opt.value());

    // On passe manuellement les deux joueurs au niveau 2 pour le test du palier 2->3
    p1.levelUp();
    p2.levelUp();
    ASSERT_EQ(p1.level(), 2);
    ASSERT_EQ(p2.level(), 2);

    // On force les deux joueurs sur la même case (5, 5)
    game::Position sharedPos = {.x = 5, .y = 5};
    world.updatePositionOnMap(p1.id(), p1.position(), sharedPos);
    p1.setPosition(sharedPos);

    world.updatePositionOnMap(p2.id(), p2.position(), sharedPos);
    p2.setPosition(sharedPos);

    // Ajout des ressources pour Niveau 2 -> 3 : 1 linemate, 1 deraumere, 1 sibur
    world.addItemOnGround(game::ItemType::Linemate, sharedPos);
    world.addItemOnGround(game::ItemType::Deraumere, sharedPos);
    world.addItemOnGround(game::ItemType::Sibur, sharedPos);

    const std::unique_ptr<ICommand> incantation = std::make_unique<Incantation>();

    // On valide que le lancement est possible
    ASSERT_TRUE(incantation->start(world, p1));

    // Exécution de l'incantation
    incantation->execute(world, p1);

    // 1. CHECK : Est-ce que TOUS les joueurs de la case ont level-up au niveau 3 ?
    EXPECT_EQ(p1.level(), 3);
    EXPECT_EQ(p2.level(), 3);

    // 2. CHECK : Est-ce que toutes les pierres requises ont été nettoyées du sol ?
    auto resourcesAfter = world.resourcesAt(sharedPos);
    EXPECT_EQ(resourcesAfter.at(static_cast<std::uint8_t>(game::ItemType::Linemate)), 0);
    EXPECT_EQ(resourcesAfter.at(static_cast<std::uint8_t>(game::ItemType::Deraumere)), 0);
    EXPECT_EQ(resourcesAfter.at(static_cast<std::uint8_t>(game::ItemType::Sibur)), 0);
}

}  // namespace zappy::server::command
