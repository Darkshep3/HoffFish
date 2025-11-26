#include <iostream>
#include "movegenerator.h"
#include <cstdlib> 

using namespace std;

int main ()
{
    Bitboard board; 
    board.displayBoard();

    U64 allies = board.getWhitePieces();
    board.display(allies);
    board.display(getKingMoves(24, allies));
    return 0;
}