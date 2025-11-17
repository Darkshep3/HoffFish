#include "movegenerator.h"
bool MoveGenerator::isAttackedSquare(Bitboard& bb, int square, bool isWhite){

}
void MoveGenerator::generateKingMoves(Bitboard& bb, long allies, long empty, bool isWhiteToMove, bool kCastle, bool qCastle, vector<Move>& moves){
    U64 king = isWhiteToMove ? bb.wking : bb.bking;
<<<<<<< HEAD
    int fromSq = get_LSB(king);
    U64 temp = getKingMoves(fromSq, allies);
    while (temp != 0){
        moves.push_back(Move(fromSq, get_LSB(temp)));
    }
    if (kCastle) {
        int sq = isWhiteToMove? 5: 61;
        int sq2 = isWhiteToMove? 6: 62;
        bool f = (((1ULL << (sq)) & empty) != 0) && !(isAttackedSquare(bb, sq, !isWhiteToMove));
        bool g = ((1ULL << (sq2)) & empty) != 0 && !(isAttackedSquare(bb, sq2, !isWhiteToMove));
        if (f && g) {
            int toSq = isWhiteToMove? 6: 62;
            moves.push_back(Move(fromSq, toSq, MoveType::CASTLING));
        }
    }
    if (qCastle) {
        int sq = isWhiteToMove? 1: 57;
        int sq2 = isWhiteToMove? 2: 58;
        int sq3 = isWhiteToMove? 3: 59;
        bool b = ((1ULL << (sq)) & empty) != 0;
        bool c = ((1ULL << sq2) & empty) != 0 && !(isAttackedSquare(bb, sq2, !isWhiteToMove));
        bool d = ((1ULL << sq3) & empty) != 0 && !(isAttackedSquare(bb, sq3, !isWhiteToMove));

        if (b && c && d){
            int toSq = isWhiteToMove ? 2 : 58;
            moves.push_back(Move(fromSq, toSq, MoveType::CASTLING));
        }
    }
}
=======
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

>>>>>>> 9519dd1a557b09321d763da09ef876859650a23f
