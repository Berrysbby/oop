#include "GameBoard.h"
#include "Exceptions.h"
#include "Ability.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>

GameBoard::GameBoard()
	:ShipMan(), Cells(), NextAttackDouble(false)
{
}

GameBoard::GameBoard(const ShipManager& shpmn, size_t width, size_t height)
	:ShipMan(shpmn), Cells(), NextAttackDouble(false)
{
	Cells.resize(height);  //Y
	for (auto& row : Cells)
		row.resize(width, { EmptyCell,CellState::Unknown,ShipState::Good }); //X
}

GameBoard::GameBoard(const GameBoard& r)
	:ShipMan(r.ShipMan), Cells(r.Cells), NextAttackDouble(r.NextAttackDouble)
{
}

GameBoard::GameBoard(GameBoard&& r)
	:ShipMan(std::move (r.ShipMan)), Cells(std::move(r.Cells)), NextAttackDouble(r.NextAttackDouble)
{
}

GameBoard& GameBoard::operator=(const GameBoard& r)
{
	ShipMan = r.ShipMan;
	Cells = r.Cells;
	NextAttackDouble = r.NextAttackDouble;
	return  *this;
}
GameBoard& GameBoard::operator=(GameBoard&& r)
{
	ShipMan = std::move(r.ShipMan);
	Cells = std::move(r.Cells);
	NextAttackDouble = r.NextAttackDouble;
	return  *this;
}

GameBoard::~GameBoard()
{
}

bool GameBoard::PlaceShip(size_t ship_index,size_t x0, size_t y0, ShipOrientation ori)
{
	Ship& ship = ShipMan[ship_index];
	size_t len = ship.Size();
	auto [w, h] = GetBoardSize();
	if (y0 >= h || x0 >= w)
		throw OutOfBoundaries();
	if (ori == ShipOrientation::Horisontal) {
		if (x0 + len > w)
			throw IllegalShipPlacement();
		if (!CanPlaceShip((int)x0 - 1, (int)y0 - 1, (int)(x0 + len + 1), (int)y0 + 1))
			throw IllegalShipPlacement();
		DrawShipHor(ship_index, x0, y0, len);
	}
	else {
		if (y0 + len > h)
			throw IllegalShipPlacement();
		if (!CanPlaceShip((int)x0 - 1, (int)y0 - 1, (int)x0 + 1, (int)(y0 + len + 1)))
			throw IllegalShipPlacement();
		DrawShipVer(ship_index, x0, y0, len);
	}
	ship.SetOrientation(ori);
	return true;
}

Ship& GameBoard::GetShip(size_t shpindx)
{
	return ShipMan[shpindx];
}

const ShipManager& GameBoard::GetShipMan() const
{
	return ShipMan;
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

std::pair<CellState, ShipState > GameBoard::Attack( size_t x, size_t y)
{
	bool doubledamage = NextAttackDouble;
	NextAttackDouble = false;
	auto [w, h] = GetBoardSize();
	if (x >= w || y >= h)
		throw OutOfBoundaries();
	int ship_index = Cells[y][x].Own;
	if (ship_index < 0)
		return { CellState::Empty,ShipState::Good };
	Ship& ship = ShipMan[ship_index];
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
	if(doubledamage)
		ship.Attack(n);
	return { CellState::Occupied,ship.State(n) };
}


void GameBoard::SetEnemyState(size_t x, size_t y, CellState cell_state,ShipState ship_state)
{
	auto [w, h] = GetBoardSize();
	if (x >= w || y >= h)
		throw std::exception();
	Cells[y][x].State = cell_state;
	Cells[y][x].EnemyShip = ship_state;
}

bool GameBoard::ApplyAbility(Ability& ab)
{
	return ab.Apply(*this);
}

void GameBoard::SetNextAttackDouble()
{
	std::cout << "used double damage" << std::endl;
	NextAttackDouble = true;
}

bool GameBoard::Scan2x2(size_t x, size_t y)
{
	auto [w, h] = GetBoardSize();
	if (x >= w-1 || y >= h-1)
		throw OutOfBoundaries();
	std::cout << "used scanner: x=" << x << " y=" << y<<std::endl;
	return Cells[y][x].Own>=0 || Cells[y+1][x].Own >= 0 || Cells[y][x+1].Own >= 0 || Cells[y+1][x+1].Own >= 0;
}

void GameBoard::MakeBombing()
{
	std::cout << "used bombing" << std::endl;
	auto [w, h] = GetBoardSize();
	size_t numtry = w * h;
	while(--numtry){
		unsigned x = std::rand() % w;
		unsigned y = std::rand() % h;
		int indx = Cells[y][x].Own;
		if (indx >= 0) {
			Ship& ship = ShipMan[indx];
			unsigned n = std::rand() % ship.Size();
			ship.Attack(n);
			return;
		}
	}
	throw InvalidBombing();
}

static const char ship_names[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void GameBoard::Display(std::ostream& os)const
{
	auto [w,h]= GetBoardSize();
	for (size_t y = 0; y < h; y++) {
		os <<std::setw(3)<< y<<":  ";
		for (size_t x = 0; x < w; x++) {
			int j = Cells[y][x].Own;
			if (j < 0) {
				os << ". ";
				continue;
			}
			const Ship& ship = ShipMan[j];
			char c = (unsigned) j >= sizeof(ship_names) ? '%' : j >= 0 ? ship_names[j] : '*';
			os << c;
			unsigned count = 0;
			if (ship.Orientation() == ShipOrientation::Horisontal) {
				while (count+1 <= x && Cells[y][x - count-1].Own == j) {
					count++;
				}
			}
			else {
				while (count + 1 <= y && Cells[y - count - 1][x].Own == j) {
					count++;
				}
			}
			ShipState state = ship.State(count);
			if (state==ShipState::Good)
				os << '+';
			else if (state == ShipState::Damaged)
				os << '-';
			else 
				os << 'X';
		}

		os << "   ";
		os << std::setw(3) << y << ":  ";
		for (size_t x = 0; x < w; x++) {
			 os << ' ';
			 auto cs= Cells[y][x].State;
			 auto ss = Cells[y][x].EnemyShip;
			 if(cs==CellState::Occupied){
				 if (ss == ShipState::Good)
					 os << '+';
				 else if (ss == ShipState::Damaged)
					 os << '-';
				 else
					 os << 'X';
			 }
			 else if (cs == CellState::Unknown) 
					 os << '?';
			 else
				 os << '.';
		}

		os << std::endl;
	}
	os << std::setw(5) << " ";
	for (int i = 0; i <w; i++)
		os << std::setw(2) << i%10;
	os << std::setw(10) << " ";
	for (int i = 0; i < w; i++)
		os << std::setw(2) << i % 10;
	os << std::endl;
}

std::ostream& operator<<(std::ostream& os, const GameBoard& gb)
{
	gb.Display(os);
	return os;
}
