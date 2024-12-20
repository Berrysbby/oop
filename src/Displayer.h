#pragma once

#include <string>
#include "GameBoard.h"
#include "Player.h"



class Display 
{
public:
	Display();
	~Display();
	void ShowBoard(const GameBoard& gb );
	void ShowMessage(const std::string&);
	void ShowGameState( const GameState&, size_t num_abilities);
};
