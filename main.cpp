#include <iostream>
#include "movegenerator.h"
#include <cstdlib> 

using namespace std;

int main ()
{
    int randomnum = rand();
    cout << sizeof(randomnum) << endl; 
    U64 wknights = 0x0000000000000042L;

    Bitboard board; 
    board.displayBoard();

    U64 allies = board.getWhitePieces();
    board.display(allies);
    board.display(getKingMoves(24, allies));
    return 0;
}