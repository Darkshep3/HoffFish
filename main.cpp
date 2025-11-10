#include <iostream>
#include "Bitboard.h"

using namespace std;

int main ()
{
    U64 wknights = 0x0000000000000042L;

    Bitboard board; 
    board.initialize();
    board.displayBoard();
    board.display(wknights);
    //board.displayBoard();
    return 0;
}