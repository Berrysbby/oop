#include "Player.h"
#include "GameBoard.h"
#include "ShipManager.h"
#include "Ship.h"
#include "AbilityManager.h"
#include "Exceptions.h"
#include <iostream>
#include <random>
#include <fstream>


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

void Game::InitializeGame()
{
    InitializeEnemyBoard();
    InitializePlayerBoard();
    InitializeAbilities();
    State.Init();
}

void Game::InitializeRound()
{
    InitializeEnemyBoard();
}

GameBoard Game::GenerateRandomBoard()
{
    ShipManager sm({ 4,3,3,2,2,2,1,1,1,1 });
    GameBoard b(sm, BoardWidth, BoardHeight);
    for (int i = 0; i < 10; i++) {
        bool newship;
        do {
            auto ori = std::rand() & 1 ? ShipOrientation::Horisontal : ShipOrientation::Vertical;
            auto s = sm[i].Size();   //ship size
            unsigned x, y;
            if (ori == ShipOrientation::Horisontal) {
                x = std::rand() % (BoardWidth - 1 - s);
                y = std::rand() % (BoardHeight - 1);
            }
            else {
                x = std::rand() % (BoardWidth - 1);
                y = std::rand() % (BoardHeight - 1 - s);
            }
            newship = false;
            try {
                newship = b.PlaceShip(i, x, y, ori);
            }
            catch (...) {
            }
        } while (!newship);
    }
    return b;
}

void Game::InitializePlayerBoard()
{
    std::cout << "Do you want to generate random board?(y,n)";
    char ans;
    std::cin >> ans;
    if (ans == 'y') {
        PlayerBoard = GenerateRandomBoard();
        return;
    }
    ShipManager sm({ 4,3,3,2,2,2,1,1,1,1 });
    GameBoard b(sm, BoardWidth, BoardHeight);
    for (int i = 0; i < 10; i++) {
        bool newship;
        do {
         //   
            auto s = sm[i].Size();   //ship size
            unsigned x, y;
            char o;
            std::cout << b<<std::endl<<s<<"-segment ship; enter X Y O(h or v): ";
            std::cin >> x >> y >> o;
            auto ori = o=='h' ? ShipOrientation::Horisontal : ShipOrientation::Vertical;
            newship = false;
            try {
                newship = b.PlaceShip(i, x, y, ori);
            }
            catch (...) {
            }
        } while (!newship);
    }
    PlayerBoard = b;
}

void Game::InitializeEnemyBoard()
{
    EnemyBoard =GenerateRandomBoard();
}

void Game::InitializeAbilities()
{
    unsigned x = std::rand() % (BoardWidth - 1 );
    unsigned y = std::rand() % (BoardHeight - 1);
    switch (std::rand() % 6) {
        case 0:
            Abilities.AddAbility(std::make_shared<DoubleDamage>());
            Abilities.AddAbility(std::make_shared<Bombing>());
            Abilities.AddAbility(std::make_shared<Scanner>(x,y));
            break;
        case 1:
            Abilities.AddAbility(std::make_shared<DoubleDamage>());
            Abilities.AddAbility(std::make_shared<Scanner>(x, y));
            Abilities.AddAbility(std::make_shared<Bombing>());
            break;
        case 2:
            Abilities.AddAbility(std::make_shared<Bombing>());
            Abilities.AddAbility(std::make_shared<Scanner>(x, y));
            Abilities.AddAbility(std::make_shared<DoubleDamage>());
            break;
        case 3:
            Abilities.AddAbility(std::make_shared<Bombing>());
            Abilities.AddAbility(std::make_shared<DoubleDamage>());
            Abilities.AddAbility(std::make_shared<Scanner>(x, y));
            break;
        case 4:
            Abilities.AddAbility(std::make_shared<Scanner>(x, y));
            Abilities.AddAbility(std::make_shared<Bombing>());
            Abilities.AddAbility(std::make_shared<DoubleDamage>());
            break;
        case 5:
            Abilities.AddAbility(std::make_shared<Scanner>(x, y));
            Abilities.AddAbility(std::make_shared<DoubleDamage>());
            Abilities.AddAbility(std::make_shared<Bombing>());
            break;
    }
}

RoundResult Game::Round()
{
    State.RoundNumber += 1;
    for (;;) {
        std::cout << std::endl;
        std::cout << PlayerBoard;
        std::cout << "Round: " << State.RoundNumber << std::endl;
        std::cout << "Your Score: " << State.UserScore << " Enemy score: " << State.EnemyScore << std::endl;
        std::cout << "Abilities: " << Abilities.Size() << std::endl;
        std::cin.clear();
        char cmd;
        std::cout << "Enter cmd (Q,T,L,S): ";
        std::cin >> cmd;
        std::cout << std::endl;
        if (cmd == 'Q')
            return RoundResult::Quit;
        if (cmd == 'L'){
            LoadGame();
            continue;
        }
        if (cmd == 'S') {
            SaveGame();
            continue;
        }
        if (cmd != 'T') {
            continue;
        }
        std::cin.clear();
        int x = -1, y = -1;
        char ability='n';
        if(!Abilities.Empty()){
            std::cout << "Enter x y ability(y or n): ";
            std::cin >> x >> y >> ability;
            std::cout << std::endl;
        }
        else {
            std::cout << "Enter x y: ";
            std::cin >> x >> y ;
            std::cout << std::endl;
        }
        if (x < 0 || y < 0)
            continue;
        UserTurn(x, y, ability == 'y'); 
        if (EnemyBoard.GetShipMan().AllDestroyed()) {
            std::cout << EnemyBoard << std::endl;
            std::cout << "Round is over. You won!"<<std::endl;
            return RoundResult::RoundOver;
        }
        EnemyTurn();
        if (PlayerBoard.GetShipMan().AllDestroyed()) {
            std::cout << PlayerBoard << std::endl;
            std::cout << "Game is over. You were defeated!" << std::endl;
            break;
        }
    }

    return RoundResult::GameOver;
}

void Game::RunGame() 
{
    for(;;){
        std::cout << "Initialize GAME" << std::endl;
        InitializeGame();
        for(;;){
            auto res= Round();
            if (res == RoundResult::Quit) {
                std::cout << "Quit" << std::endl;
                return;
            }
            if (res == RoundResult::GameOver)
                break;
            if (res == RoundResult::RoundOver) {
                std::cout << "Initialize ROUND" << std::endl;
                InitializeRound();
            }
        }
    }
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
            std::cout << "scanner found ship" << std::endl;
    }

    //perform attack
   auto [cs,ss] = EnemyBoard.Attack(x, y);
   PlayerBoard.SetEnemyState(x, y, cs, ss);
    //return result
    if (cs != CellState::Occupied)
        return false;
    State.UserScore += 1;
    if (ss == ShipState::Destroyed)
        Abilities.AddRandomAbility(PlayerBoard);
    return true;
}

void Game::SetNextAttackDouble()
{
    std::cout << "used double damage" << std::endl;
    EnemyBoard.SetNextAttackDouble();
}

bool Game::Scan2x2(size_t x, size_t y)
{
    std::cout << "used scanner: x=" << x << " y=" << y << std::endl;
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
        std::cout << "used bombing: " << x << " " << y << std::endl;
        auto [cs, ss] = EnemyBoard.Attack(x, y);
        PlayerBoard.SetEnemyState(x, y, cs, ss);
        State.UserScore += 1;
    }
    catch (...) {
        std::cout << "used bombing: error" << std::endl;
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

