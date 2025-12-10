#pragma once
#include "GameState.h"
#include "Move.h"
#include "pieces.h"
#include <vector>

class MoveGenerator {
public: 
//Diana 
    static vector<Move> generatePseudoMoves(GameState& state);
    static vector<Move> generateLegalMoves(GameState& state); 
//private:
    static void generatePawnMoves(Bitboard& bb, U64 enemy, U64 empty, int enPassant, bool isWhiteToMove, vector<Move>& moves);
    static void generateKnightMoves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves);
    static void generateKingMoves(Bitboard& bb, U64 allies, U64 empty, bool isWhiteToMove, bool kCastle, bool qCastle, vector<Move>& moves);
    static void generateBishopMoves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves);
    static void generateRookMoves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves);
    static void generateQueenMoves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves);

    static bool isAttackedSquare( Bitboard& bb, int square, bool isWhite);
    static bool is_in_check(bool white_to_move, Bitboard bb); 


};