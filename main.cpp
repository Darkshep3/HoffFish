#include <iostream>
#include "movegenerator.cpp";
#include "Bitboard.h"

using namespace std;

int main ()
{
    U64 wknights = 0x0000000000000042L;

    Bitboard board; 
    board.initialize();
    board.displayBoard();

    U64 allies = board.getWhitePieces();
    board.display(allies);
    board.display(getKingMoves(24, allies));
    return 0;
}