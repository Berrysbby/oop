#include "GameBoard.h"
#include "Exceptions.h"
#include "Ability.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>

GameBoard::GameBoard(size_t width, size_t height)
{
	Cells.resize(height);  //Y
	for (auto& row : Cells)
		row.resize(width, { EmptyCell,CellState::Unknown}); //X
	NextAttackDouble = false;
}

GameBoard::GameBoard(const GameBoard& r)
{
	Cells = r.Cells;
	NextAttackDouble = r.NextAttackDouble;
}
GameBoard::GameBoard(GameBoard&& r)
{
	Cells = std::move(r.Cells);
	NextAttackDouble = r.NextAttackDouble;
}
GameBoard& GameBoard::operator=(const GameBoard& r)
{
	Cells = r.Cells;
	NextAttackDouble = r.NextAttackDouble;
	return  *this;
}
GameBoard& GameBoard::operator=(GameBoard&& r)
{
	Cells = std::move(r.Cells);
	NextAttackDouble = r.NextAttackDouble;
	return  *this;
}

GameBoard::~GameBoard()
{
}

bool GameBoard::PlaceShip(ShipManager& shipman,size_t ship_index,size_t x0, size_t y0, ShipOrientation ori)
{
	Ship& ship = shipman[ship_index];
	size_t len = ship.Size();
	auto [w, h] = GetBoardSize();
	if (y0 >= h || x0 >= w)
		throw OutOfBoundaries();
	if (ori == ShipOrientation::Horisontal) {
		if (x0 + len >= w)
			throw IllegalShipPlacement();
		if (!CanPlaceShip((int)x0 - 1, (int)y0 - 1, (int)(x0 + len + 1), (int)y0 + 1))
			throw IllegalShipPlacement();
		DrawShipHor(ship_index, x0, y0, len);
	}
	else {
		if (y0 + len >= h)
			throw IllegalShipPlacement();
		if (!CanPlaceShip((int)x0 - 1, (int)y0 - 1, (int)x0 + 1, (int)(y0 + len + 1)))
			throw IllegalShipPlacement();
		DrawShipVer(ship_index, x0, y0, len);
	}
	ship.SetOrientation(ori);
	return true;
}

bool GameBoard::CanPlaceShip(int x0, int y0, int x1, int y1)
{
	auto [w, h] = GetBoardSize();
	if (y0 >= (int)h)
		y0 = (int)h - 1;
	if (x0 >= (int)w)
		x0 = (int)w - 1;
	if (y1 >= (int)h)
		y1 = (int)h - 1;
	if (x1 >= (int)w)
		x1 = (int)w - 1;
	if (y0 < 0)
		y0 = 0;
	if (x0 < 0)
		x0 = 0;
	if (y1 < 0)
		y1 = 0;
	if (x1 < 0)
		x1 = 0;
	for (int x = x0; x <= x1;++x) {
		for (int y = y0; y <= y1; ++y) {
			if (Cells[y][x].Own != EmptyCell && Cells[y][x].Own != SurrCell)
				return false;
		}
	}
	return true;
}

void GameBoard::MarkHor(int value, int x0, int y0, int x1)
{
	auto [w, h] = GetBoardSize();
	if(x0>=(int)w || x1<0 || y0<0 || y0>= (int)h)
		return;
	if (x0 < 0)
		x0 = 0;
	if (x1 >= (int)w)
		x1 = (int)w - 1;
	for (int x = x0; x <= x1; ++x)
		Cells[y0][x].Own = value;
}

void GameBoard::MarkVer(int value, int x0, int y0, int y1)
{
	auto [w, h] = GetBoardSize();
	if (y0 >= (int)h || y1 < 0 || x0 < 0 || x0 >= (int)w)
		return;
	if (y0 < 0)
		y0 = 0;
	if (y1 >= (int)h)
		y1 = (int)h - 1;
	for (int y = y0; y <= y1; ++y)
		Cells[y][x0].Own = value;
}

void GameBoard::DrawShipHor(size_t ship_index, size_t x0, size_t y0, size_t len)
{
	MarkHor((int)ship_index, (int)x0, (int)y0, (int)(x0 + len - 1));
	MarkHor(SurrCell, (int)x0, (int)y0 - 1, (int)(x0 + len - 1));
	MarkHor(SurrCell, (int)x0, (int)y0 + 1, (int)(x0 + len - 1));
	MarkVer(SurrCell, (int)x0 - 1, (int)y0 - 1, (int)y0 + 1);
	MarkVer(SurrCell, (int)(x0 + len), (int)y0 - 1, (int)y0 + 1);
}

void GameBoard::DrawShipVer(size_t ship_index, size_t x0, size_t y0, size_t len)
{
	MarkVer((int)ship_index, (int)x0, (int)y0, (int)(y0 + len - 1));
	MarkVer(SurrCell, (int)x0 - 1, (int)y0, (int)(y0 + len - 1));
	MarkVer(SurrCell, (int)x0 + 1, (int)y0, (int)(y0 + len - 1));
	MarkHor(SurrCell, (int)x0 - 1, (int)y0 - 1, (int)x0 + 1);
	MarkHor(SurrCell, (int)x0 - 1, (int)(y0 + len), (int)x0 + 1);
}

std::pair<size_t, size_t> GameBoard::GetBoardSize() const
{
	size_t h = Cells.size();
	if (h == 0)
		throw EmptyBoard();
	size_t w = Cells[0].size();
	if (w == 0)
		throw EmptyBoard();
	return {w,h};
}

CellState GameBoard::Attack(ShipManager& shipman, size_t x, size_t y)
{
	auto [w, h] = GetBoardSize();
	if (x >= w || y >= h)
		throw OutOfBoundaries();
	int ship_index = Cells[y][x].Own;
	if (ship_index < 0)
		return CellState::Empty;
	Ship& ship = shipman[ship_index];
	size_t n = 0;
	if (ship.Orientation() == ShipOrientation::Horisontal) {
		while (x>n && Cells[y][x - n - 1].Own == ship_index)
			n++;
	}
	else {
		while (y > n && Cells[y-n-1][x].Own == ship_index)
			n++;
	}
	ship.Attack(n);
	return CellState::Occupied;
}

/*
void GameBoard::SetEnemyState(size_t x, size_t y, CellState state)
{
	auto [w, h] = GetBoardSize();
	if (x >= w || y >= h)
		throw std::exception();
	Cells[y][x].State = state;
}
*/

bool GameBoard::ApplyAbility(Ability& ab)
{
	return ab.Apply(*this);
}

void GameBoard::SetNextAttackDouble()
{
	NextAttackDouble = true;
}

bool GameBoard::Scan2x2(size_t x, size_t y)
{
	auto [w, h] = GetBoardSize();
	if (x >= w-1 || y >= h-1)
		throw OutOfBoundaries();
	return Cells[y][x].Own>=0 || Cells[y+1][x].Own >= 0 || Cells[y][x+1].Own >= 0 || Cells[y+1][x+1].Own >= 0;
}

void GameBoard::MakeBombing(ShipManager& shipman)
{
	auto [w, h] = GetBoardSize();
	size_t numtry = w * h;
	while(--numtry){
		unsigned x = std::rand() % w;
		unsigned y = std::rand() % h;
		int indx = Cells[y][x].Own;
		if (indx >= 0) {
			Ship& ship = shipman[indx];
			unsigned n = std::rand() % ship.Size();
			ship.Attack(n);
			return;
		}
	}
	throw InvalidBombing();
}

static const char ship_names[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void GameBoard::Display(ShipManager& shipman)
{
	auto [w,h]= GetBoardSize();
	for (size_t y = 0; y < h; y++) {
		for (size_t x = 0; x < w; x++) {
			int j = Cells[y][x].Own;
			if (j < 0) {
				std::cout << ". ";
				continue;
			}
			Ship& ship = shipman[j];
			char c = (unsigned) j >= sizeof(ship_names) ? '%' : j >= 0 ? ship_names[j] : '*';
			std::cout << c;
			unsigned count = 0;
			if (ship.Orientation() == ShipOrientation::Horisontal) {
				while (count+1 <= x && Cells[y][x - count-1].Own == j) {
					count++;
				}
			}
			else {
				while (count + 1 <= x && Cells[y - count - 1][x].Own == j) {
					count++;
				}
			}
			ShipState state = ship.State(count);
			if (state==ShipState::Good)
				std::cout << ')';
			else if (state == ShipState::Damaged)
				std::cout << '(';
			else 
				std::cout << 'X';
		}
		std::cout << std::endl;
	}
}
