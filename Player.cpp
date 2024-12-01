#include "Player.h"
#include "GameBoard.h"
#include "ShipManager.h"
#include "Ship.h"
#include "Ability.h"
#include "Exceptions.h"
#include<iostream>

void DisplayGameState(GameState state) {
    std::cout << "Раунд: " << state.roundNumber << std::endl;
    std::cout << "Ваши очки: " << state.userScore << " | Очки врага: " << state.enemyScore << std::endl;
    std::cout << "Способность " << (state.AbilityCount != 0 ? "доступно способностей" : "нет способностей") << std::endl;
}

