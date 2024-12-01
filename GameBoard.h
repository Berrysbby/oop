#pragma once

#include "Ship.h"
#include "ShipManager.h"
#include <utility>
#include <vector>

class Ability;

enum class CellState { Unknown, Empty, Occupied };

class GameBoard
{
private:  //типы 
	enum { EmptyCell = -1, SurrCell = -2 };
	struct Cell {
		int Own;
		CellState State;
	};
private: //данные
	std::vector< std::vector< Cell > > Cells;
	bool NextAttackDouble;
private:  //методы
	bool CanPlaceShip(int x0, int y0, int x1, int y1);
	void MarkHor(int value,int x0, int y0, int x1);
	void MarkVer(int value, int x0, int y0, int y1);
	void DrawShipHor(size_t ship_index, size_t x0, size_t y0, size_t len);
	void DrawShipVer(size_t ship_index, size_t x0, size_t y0, size_t len);

public: //интерфейс
	GameBoard( size_t width, size_t height);
	GameBoard(const GameBoard&);
	GameBoard( GameBoard&&);
	GameBoard& operator=(const GameBoard&);
	GameBoard& operator=(GameBoard&&);
	~GameBoard();
	std::pair<size_t, size_t> GetBoardSize() const;
	bool PlaceShip(ShipManager& shm,size_t ship_index, size_t x, size_t y, ShipOrientation ori );
	CellState Attack(ShipManager& shm,size_t x, size_t y);
	//void SetEnemyState(size_t x, size_t y,CellState state);
	bool ApplyAbility(Ability&);
	void SetNextAttackDouble();
	bool Scan2x2(size_t x, size_t y);
	void MakeBombing(ShipManager& shm);
	void Display(ShipManager& shm);
};
