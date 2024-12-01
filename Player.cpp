#include "Player.h"
#include "GameBoard.h"
#include "ShipManager.h"
#include "Ship.h"
#include "Ability.h"
#include "Exceptions.h"
#include<iostream>

void DisplayGameState(GameState state) {
    std::cout << "�����: " << state.roundNumber << std::endl;
    std::cout << "���� ����: " << state.userScore << " | ���� �����: " << state.enemyScore << std::endl;
    std::cout << "����������� " << (state.AbilityCount != 0 ? "�������� ������������" : "��� ������������") << std::endl;
}

