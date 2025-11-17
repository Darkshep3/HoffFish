#include "movegenerator.h"

void MoveGenerator::generateKingMoves(Bitboard& bb, long allies, long empty, bool isWhiteToMove, bool kCastle, bool qCastle, vector<Move>& moves){
    U64 king = isWhiteToMove ? bb.wking : bb.bking;
    //moves.push_back()
}
void MoveGenerator::generatePawnMoves(Bitboard& bb, U64 enemy, U64 empty, int enPassant, bool isWhiteToMove, vector<Move>& moves)
{
    U64 pawns = white ? bb.whitePawns : bb.blackPawns;
    int direction      = white ? 8 : -8;    
    int startRank      = white ? 1 : 6;     
    int promoRank      = white ? 6 : 1;    
    int epCaptureLeft  = white ? 7 : -9;    
    int epCaptureRight = white ? 9 : -7; 
}

