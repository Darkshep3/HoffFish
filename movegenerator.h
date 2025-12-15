#pragma once
#include "GameState.h"
#include "Move.h"
#include "Pieces.h"
#include <vector>

class MoveGenerator 
{
public: 
//Diana 
    static vector<Move> generate_pseudo_moves(GameState& state);
    static vector<Move> generate_legal_moves(GameState& state); 
//private:
    static void generate_pawn_moves(Bitboard& bb, U64 enemy, U64 empty, int enPassant, bool isWhiteToMove, vector<Move>& moves);
    static void generate_knight_moves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves);
    static void generate_king_moves(Bitboard& bb, U64 allies, U64 empty, bool isWhiteToMove, bool kCastle, bool qCastle, vector<Move>& moves);
    static void generate_bishop_moves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves);
    static void generate_rook_moves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves);
    static void generate_queen_moves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves);

    static bool is_attacked_square( Bitboard& bb, int square, bool isWhite);
    static bool is_in_check(bool white_to_move, Bitboard bb); 


};