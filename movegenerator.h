#pragma once
#include "GameState.h"
#include "Move.h"
#include "pieces.h"
#include <vector>

class MoveGenerator {
public: 
//Diana 
    static vector<Move> generatePseudoMoves(const GameState& state);
    static vector<Move> generateLegalMoves(GameState& state); 
//private:
    static void generatePawnMoves(const Bitboard& bb, U64 enemy, U64 empty, int enPassant, bool isWhiteToMove, vector<Move>& moves);
    static void generateKnightMoves(const Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves);
    static void generateKingMoves(const Bitboard& bb, U64 allies, U64 empty, bool isWhiteToMove, bool kCastle, bool qCastle, vector<Move>& moves);
    static void generateBishopMoves(const Bitboard& bb, U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves);
    static void generateRookMoves(const Bitboard& bb, U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves);
    static void generateQueenMoves(const Bitboard& bb, U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves);

    static bool isAttackedSquare(const Bitboard& bb, int square, bool isWhite);
    bool is_in_check(bool white_to_move, Bitboard bb); 


};