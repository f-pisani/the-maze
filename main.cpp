#include "TheMaze.hpp"

int main(int argc, char *argv[])
{
    TheMaze Game;

    while(Game.isOpen())
        Game.update();

    return 0;
}
