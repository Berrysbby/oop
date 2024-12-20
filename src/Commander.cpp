#include "Commander.h"
#include <iostream>
#include <iomanip>

Commander::Commander()
{
}

Commander::~Commander()
{
}

bool Commander::AskPlayerboardType()
{
    std::cout << "Do you want to generate random board?(y,n)";
    char ans;
    std::cin >> ans;
    if (ans == 'y')
	    return true;
    return false;
}

Commander::ShipPlacement Commander::AskShipPlacement(size_t s)
{
    unsigned x, y;
    char o;
    std::cout   << s << "-segment ship; enter X Y O(h or v): ";
    std::cin >> x >> y >> o;
    return ShipPlacement{ x,y,o == 'v' };
}

Commander::NextAction Commander::AskNextAction(bool ab)
{
    NextAction na;
    na.x = na.y = 0;
    na.cmd = Cmd::Error;
    na.ab = false;
    std::cout << "Enter cmd: Q/L/S/x y ability(y or n): ";
    int ch=std::cin.peek();
    if (ch == 'Q' || ch == 'q')
        na.cmd = Cmd::Quit;
    if (ch == 'L' || ch == 'l')
        na.cmd = Cmd::Load;
    if (ch == 'S' || ch == 's')
        na.cmd = Cmd::Save;
    if (na.cmd != Cmd::Error) {
        do {
            ch = std::cin.get();
        } while (ch != EOF && ch != '\n');
        std::cin.clear();
        return na;
    }
    std::cin.clear();
    int x = -1, y = -1;
    char ability = 'n';
    if (ab) {
        std::cin >> x >> y >> ability;
        std::cout << std::endl;
    }
    else {
        std::cin >> x >> y;
        std::cout << std::endl;
    }
    if (x >= 0 && y >= 0) {
        na.cmd = Cmd::Move;
        na.x = x;
        na.y = y;
        na.ab = 'y' == ability;
    }
    do {
        ch = std::cin.get();
    } while (ch != EOF && ch != '\n');
    std::cin.clear();
    return na;
}
