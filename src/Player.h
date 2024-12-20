#pragma once
#include "GameBoard.h"
#include "ShipManager.h"
#include "Ship.h"
#include "AbilityManager.h"
#include <ostream>
#include <istream>
#include <string>

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

enum class RoundResult{Quit,GameOver,RoundOver,Continue};
class Game {
private:
    size_t BoardWidth = 10;
    size_t BoardHeight = 10;
    GameBoard   PlayerBoard;
    GameBoard   EnemyBoard;
    GameState State;
    AbilityManager Abilities;
    std::string AbilityMessage;

private:
    GameBoard GenerateRandomBoard();
    void InitializePlayerBoard(bool gen);
    void InitializeEnemyBoard();
    bool UserTurn(size_t x, size_t y, bool use_ability);
    bool EnemyTurn();
    bool ApplyAbility(Ability&);

    
public:
    Game();
    ~Game();
    void InitializeGame(bool gen);
    GameBoard& GetPlayerBoard();
    size_t GetAbilitiesSize()const;
    const GameState& GetGameState();
    std::string GetAbilityMessage();
    bool PlacePlayerShip(int indx,size_t x, size_t y, ShipOrientation );
    void InitializeRound();
    //раунд
    RoundResult Turn(size_t x, size_t y, bool ab);
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