#include "Player.h"
#include "GameBoard.h"
#include "ShipManager.h"
#include "Ship.h"
#include "AbilityManager.h"
#include "Exceptions.h"
//#include <iostream>
#include <random>
#include <fstream>
#include <sstream>


void GameState::Init()
{
    UserScore = EnemyScore = RoundNumber=0;
    EnemyTurn = false;
}

void GameState::Save(std::ostream& str)
{
    str << UserScore << " " << EnemyScore << " " << RoundNumber << std::endl;
}

void GameState::Load(std::istream& str)
{
    str >> UserScore >> EnemyScore >> RoundNumber;
    EnemyTurn = false;
}

Game::Game()
{
   
}

Game::~Game()
{
}

void Game::InitializeGame(bool gen)
{
    InitializeEnemyBoard();
    InitializePlayerBoard(gen);
    Abilities.InitForNewGame(BoardWidth,BoardHeight);
    State.Init();
}

GameBoard& Game::GetPlayerBoard()
{
    return PlayerBoard;
}

size_t Game::GetAbilitiesSize() const
{
    return Abilities.Size();
}

const GameState& Game::GetGameState()
{
    return State;
}

std::string Game::GetAbilityMessage()
{
    return std::move(AbilityMessage);
}

bool Game::PlacePlayerShip(int indx,size_t x, size_t y, ShipOrientation ori)
{
    bool newship = false;
    try {
        newship = PlayerBoard.PlaceShip(indx, x, y, ori);
    }
    catch (...) {
    }
    return newship;
}

void Game::InitializeRound()
{
    InitializeEnemyBoard();
}

GameBoard Game::GenerateRandomBoard()
{
    ShipManager sm({ 4,3,3,2,2,2,1,1,1,1 });
    GameBoard b(sm, BoardWidth, BoardHeight);
    b.PopulateShipsRandom();
    return b;
}

void Game::InitializePlayerBoard(bool gen)
{
    if (gen) {
        PlayerBoard = GenerateRandomBoard();
        return;
    }
    ShipManager sm({ 4,3,3,2,2,2,1,1,1,1 });
    PlayerBoard = GameBoard(sm, BoardWidth, BoardHeight);
}

void Game::InitializeEnemyBoard()
{
    EnemyBoard =GenerateRandomBoard();
}


RoundResult Game::Turn(size_t x, size_t y, bool ab)
{
    State.RoundNumber += 1;
    UserTurn(x, y, ab); 
    if (EnemyBoard.GetShipMan().AllDestroyed()) {
        return RoundResult::RoundOver;
    }
    EnemyTurn();
    if (PlayerBoard.GetShipMan().AllDestroyed()) {
        return RoundResult::GameOver;
    }
    return RoundResult::Continue;
}


bool Game::UserTurn(size_t x, size_t y, bool use_ability)
{
    // check state
    if (State.EnemyTurn)
        throw InternalError();
    State.EnemyTurn = true;
    // use ability
    if (use_ability) {
        auto ability = Abilities.GetAbility();
        if(ApplyAbility(*ability))
            AbilityMessage+=": scanner found ship";
    }

    //perform attack
   auto [cs,ss] = EnemyBoard.Attack(x, y);
   PlayerBoard.SetEnemyState(x, y, cs, ss);
    //return result
    if (cs != CellState::Occupied)
        return false;
    State.UserScore += 1;
    if (ss == ShipState::Destroyed) {
        auto [w, h] = PlayerBoard.GetBoardSize();
        Abilities.AddRandomAbility(w, h);
    }
    return true;
}

void Game::SetNextAttackDouble()
{
    AbilityMessage = "used double damage";
    EnemyBoard.SetNextAttackDouble();
}

bool Game::Scan2x2(size_t x, size_t y)
{
    std::ostringstream os;
    os << "used scanner: x=" << x << " y=" << y;
    AbilityMessage = os.str();
    return EnemyBoard.Scan2x2(x, y);
}

void Game::MakeBombing()
{
    try {
        auto [ship_index, x, y] = EnemyBoard.LookupBombing();
        const Ship& ship = EnemyBoard.GetShip(ship_index);
        for (int j = 0; j < 10; ++j) {
            unsigned m = std::rand() % ship.Size();
            if (ship.Orientation() == ShipOrientation::Horisontal)
                x += m;
            else
                y += m;
            if (ship.State(m) != ShipState::Destroyed)
                break;
        }
        std::ostringstream os;
        os << "used bombing: " << x << " " << y;
        AbilityMessage = os.str();
        auto [cs, ss] = EnemyBoard.Attack(x, y);
        PlayerBoard.SetEnemyState(x, y, cs, ss);
        State.UserScore += 1;
    }
    catch (...) {
        AbilityMessage = "used bombing: error";
    }
}

bool Game::EnemyTurn()
{
    // check state
    if (!State.EnemyTurn)
        throw InternalError();
    State.EnemyTurn = false;
    auto [w, h] = PlayerBoard.GetBoardSize();
    //generate random x and y for attack
    unsigned x = std::rand() % (w - 1);
    unsigned y = std::rand() % (h - 1);
    //perform attack
    auto [cs, ss] = PlayerBoard.Attack(x, y);
    EnemyBoard.SetEnemyState(x, y, cs, ss);
    //return result
    if (cs != CellState::Occupied)
        return false;
    State.EnemyScore += 1;
    return true;
}

bool Game::ApplyAbility(Ability& ab)
{
    return ab.Apply(*this);
}

void Game::SaveGame()
{
    std::ofstream of("saved_game", std::ios_base::trunc);
    of << BoardWidth << " " << BoardHeight<<std::endl;
    PlayerBoard.Save(of);
    EnemyBoard.Save(of);
    State.Save(of);
    Abilities.Save(of);
}

void Game::LoadGame()
{
    std::ifstream f("saved_game");
    f>> BoardWidth>>BoardHeight;
    PlayerBoard.Load(f);
    EnemyBoard.Load(f);
    State.Load(f);
    Abilities.Load(f);
}

