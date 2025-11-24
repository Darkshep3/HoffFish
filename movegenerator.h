#pragma once
#include "GameState.h"
#include "Move.h"
#include "pieces.h"
#include <vector>

class MoveGenerator {
public: 
    static vector<Move> generateLegalMoves(GameState& state);
    static vector<Move> generatePseudoMoves(const GameState& state);
private:
    static void generatePawnMoves(Bitboard& bb, U64 enemy, U64 empty, int enPassant, bool isWhiteToMove, vector<Move>& moves);
    static void generateKnightMoves(Bitboard& bb,  U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves);
    static void generateBishopMoves(Bitboard& bb, U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves);
    static void generateKingMoves(Bitboard& bb, long allies, long empty, bool isWhiteToMove, bool kCastle, bool qCastle, vector<Move>& moves);
    static void generateSlidingMoves(Bitboard& bb, bool white, vector<Move>& moves);
    static bool isAttackedSquare(Bitboard& bb, int square, bool isWhite);


};