#pragma once
#include "GameBoard.h"
#include "ShipManager.h"
#include "Ship.h"
#include "AbilityManager.h"
#include <ostream>
#include <istream>

struct GameState {
    int UserScore;
    int EnemyScore;
    int RoundNumber;
    bool EnemyTurn;
    //    bool AbilityFlag;
    //int AbilityCount;
    void Init();
    void Save(std::ostream& str);
    void Load(std::istream& str);
};

enum class RoundResult{Quit,GameOver,RoundOver};
class Game {
private:
    size_t BoardWidth = 10;
    size_t BoardHeight = 10;
    GameBoard   PlayerBoard;
    GameBoard   EnemyBoard;
    GameState State;
    AbilityManager Abilities;

private:
    void InitializeGame();
    void InitializeRound();
    GameBoard GenerateRandomBoard();
    void InitializePlayerBoard();
    void InitializeEnemyBoard();
    void InitializeAbilities();
    //раунд
    RoundResult Round();
    bool UserTurn(size_t x, size_t y, bool use_ability);
    bool EnemyTurn();
    bool ApplyAbility(Ability&);

    
public:
    Game();
    ~Game();
    //вся игра
    void RunGame();
    //сохранение
    void SaveGame();
    //загрузка
    void LoadGame();
    //для использования ability
    void SetNextAttackDouble();
    bool Scan2x2(size_t x, size_t y);
    void MakeBombing();

public:  // для тестов
    const GameBoard& GetEnemyBoard()const { return EnemyBoard; }

};