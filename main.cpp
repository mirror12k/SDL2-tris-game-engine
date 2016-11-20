
#include <stdio.h>


#include "tris/engine.hpp"

int main ()
{
    printf("hello world\n");
    tris::engine my_game("test engine", 800, 640, 60);
    my_game.run();

    return 0;
}
