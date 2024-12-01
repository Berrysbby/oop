all: seabattle

seabattle: main.o GameBoard.o Ship.o ShipManager.o Ability.o Exceptions.o 
	g++ -o seabattle main.o GameBoard.o Ship.o ShipManager.o Ability.o Exceptions.o

main.o: GameBoard.h
	g++ -c -std=c++17 main.cpp 

GameBoard.o: GameBoard.cpp GameBoard.h Ship.h ShipManager.h Exceptions.h Ability.h
	g++ -c -std=c++17 GameBoard.cpp
 	
Ship.o: Ship.cpp Ship.h
	g++ -c -std=c++17 Ship.cpp

ShipManager.o: ShipManager.cpp ShipManager.h
	g++ -c -std=c++17 ShipManager.cpp

Ability.o: Ability.cpp Ability.h Exceptions.h ShipManager.h
	g++ -c -std=c++17 Ability.cpp

Exceptions.o: Exceptions.cpp Exceptions.h 
	g++ -c -std=c++17 Exceptions.cpp
