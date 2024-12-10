#pragma once

#include "Ship.h"
#include "ShipManager.h"
#include <utility>
#include <ostream>
#include <istream>
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
		ShipState EnemyShip;
	};
private: //данные
	ShipManager ShipMan;
	std::vector< std::vector< Cell > > Cells;
	bool NextAttackDouble;
private:  //методы
	void SetBoardSize(size_t w,size_t h);
	bool CanPlaceShip(int x0, int y0, int x1, int y1);
	void MarkHor(int value,int x0, int y0, int x1);
	void MarkVer(int value, int x0, int y0, int y1);
	void DrawShipHor(size_t ship_index, size_t x0, size_t y0, size_t len);
	void DrawShipVer(size_t ship_index, size_t x0, size_t y0, size_t len);

public: //интерфейс
	GameBoard();
	GameBoard( const ShipManager& shpmn,size_t width, size_t height);
	GameBoard(const GameBoard&);
	GameBoard( GameBoard&&);
	GameBoard& operator=(const GameBoard&);
	GameBoard& operator=(GameBoard&&);
	~GameBoard();
	std::pair<size_t, size_t> GetBoardSize() const;
	bool PlaceShip(size_t ship_index, size_t x, size_t y, ShipOrientation ori );
	Ship& GetShip(size_t shpindx);
	const ShipManager& GetShipMan()const;
	std::pair<CellState,ShipState > Attack(size_t x, size_t y);
	void SetEnemyState(size_t x, size_t y,CellState, ShipState);
	void SetNextAttackDouble();
	bool Scan2x2(size_t x, size_t y);
	struct LookupBombingResult { unsigned ship_index, x, y; };
	LookupBombingResult LookupBombing();
	void Display(std::ostream&)const;
	void Save(std::ostream&);
	void Load(std::istream&);
};

std::ostream& operator <<(std::ostream&, const GameBoard&);
