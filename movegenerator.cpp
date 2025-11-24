#include "movegenerator.h"
bool MoveGenerator::isAttackedSquare(Bitboard& bb, int square, bool isWhite){

}
void MoveGenerator::generateKingMoves(Bitboard& bb, long allies, long empty, bool isWhiteToMove, bool kCastle, bool qCastle, vector<Move>& moves){
    U64 king = isWhiteToMove ? bb.wking : bb.bking;
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

//Diana 
<<<<<<< HEAD
void MoveGenerator::generateKnightMoves(Bitboard& bb,  U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves){
=======
void MoveGenerator::generateKnightMoves(Bitboard& bb, long allies, bool isWhiteToMove, vector<Move>& moves)
{
    U64 knights = isWhiteToMove ? bb.wknights : bb.bknights;
    
    //loop thru each knight
    while(knights != 0)
    {
        int from_sq = get_LSB(knights);
        clear_LSB(knights);
        //remove current knight from bitboard
>>>>>>> refs/remotes/origin/main

        U64 temp = getKnightMoves(from_sq, allies);

        while(temp != 0)
        {
            int to_sq = get_LSB(temp);
            clear_LSB(temp);

            moves.push_back(Move(from_sq,to_sq));
        }
    }
}


//Ethan  

/*void MoveGenerator::generatePawnMoves(Bitboard& bb, U64 enemy, U64 empty, int enPassant, bool isWhiteToMove, vector<Move>& moves)
{
    U64 pawns = white ? bb.whitePawns : bb.blackPawns;
    int direction      = white ? 8 : -8;    
    int startRank      = white ? 1 : 6;     
    int promoRank      = white ? 6 : 1;    
    int epCaptureLeft  = white ? 7 : -9;    
    int epCaptureRight = white ? 9 : -7; 
}*/

// starting file -> 2 pawns
//  en passant
//  single file
//   =

//Arush 
//Bishop Move Generatoin 
// Rooks + queens if you have time  
void MoveGenerator::generateBishopMoves(Bitboard& bb, U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves){
    U64 bishops = isWhiteToMove ? bb.wbishops : bb.bbishops;
    vector<int> fromSqs;
    while (bishops != 0) {
        fromSqs.push_back(get_LSB(bishops));
        clear_bit(bishops, get_LSB(bishops));
    }

    for (int i=0; i<fromSqs.size(); i++) {
        int fromSq = fromSqs[i];
        int rank = fromSq / 8;
        int file = fromSq % 8;
        for (int j=-1; j<=1; j+=2) {
            for (int k=-1; k<=1; k+=2) {
                int mult = 1;
                int toRank = rank + j * mult;
                int toFile = file + k * mult; 
                while (toRank >= 0 && toRank <= 7 && toFile >= 0 && toFile <= 7) {
                    if (((1ULL << (toRank * 8 + toFile)) & enemy) != 0) {
                        moves.push_back(Move(fromSq, toRank * 8 + toFile, MoveType::CAPTURES));
                        toRank = 9;
                    } else if (((1ULL << (toRank * 8 + toFile)) & empty) != 0){
                        moves.push_back(Move(fromSq, toRank * 8 + toFile, MoveType::NORMAL));
                        mult++;
                        toRank = rank + j * mult;
                        toFile = file + k * mult;                         
                    } else {
                        toRank = 9;
                    }
                }
            }
        }
    }

}

