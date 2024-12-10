#pragma once
#include "GameBoard.h"
#include "ShipManager.h"
#include "Ship.h"
#include "AbilityManager.h"

struct GameState {
    int UserScore;
    int EnemyScore;
    int RoundNumber;
    bool EnemyTurn;
    //    bool AbilityFlag;
    //int AbilityCount;
    void Init();
};


class Game {
private:
    size_t BoardWidth = 10;
    size_t BoardHeight = 10;
    GameBoard   PlayerBoard;
    GameBoard   EnemyBoard;
    GameState State;
    AbilityManager Abilities;
    bool isGameOver;

private:
    GameBoard GenerateRandomBoard();
    void InitializePlayerBoard();
    void InitializeEnemyBoard();
    void InitializeAbilities();
    //состояние игры: счет, количество своих кораблей, кол-во способностей
    void DisplayGameState();
    bool UserTurn(size_t x, size_t y, bool use_ability);
    bool EnemyTurn();
    //начало нового раунда
    void ResetRound();
    
    
public:
    Game();
    ~Game();
    //начало игры
    void InitializeGame();
    //раунд
    bool Round();
    //сохранение
    void SaveGame();
    //загрузка
    void LoadGame();

public:  // для тестов
    const GameBoard& GetEnemyBoard()const { return EnemyBoard; }

};