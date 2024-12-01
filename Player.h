#pragma once
#include "GameBoard.h"
#include "ShipManager.h"
#include "Ship.h"
#include "Ability.h"

class GameState {
public:
    int userScore;
    int enemyScore;
    int roundNumber;
    bool AbilityFlag;
    int AbilityCount;

public:
    GameState(int userScore = 20, int enemyScore = 20, int roundNumber = 1, bool AbilityFlag = false, int AbilityCount = 3)  {}
};


class Game {
private:
    GameBoard   PlayerBoard;
    GameBoard   EnemyBoard;
    GameState state;
    bool isGameOver;

private:
    //������ ����
    void GameStart();
    void InitializeGame();
    //��������� ����: ����, ���������� ����� ��������, ���-�� ������������
    void DisplayGameState();
    bool UserTurn();
    bool EnemyTurn();
    //������ ������ ������
    void ResetRound();
    
public:
    //����������
    void SaveGame();
    //��������
    void LoadGame();
};