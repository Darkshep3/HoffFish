#pragma once
#include <cstdint>
#include <array> 
#include "Magic.h"

using U64 = uint64_t;
using namespace std;

//precomputed King / Move attacks lookup tables 


constexpr U64 compute_king_moves(int square)
{
    U64 bitboard = 1ULL << square;
    U64 moves = 0ULL; 

    int rank = square/8;
    int file = square % 8;

    if (rank < 7) moves |= bitboard << 8;   // to go up
    if (rank > 0) moves |= bitboard >> 8;  // go down
    if (file < 7) moves |= bitboard << 1;  // go right
    if (file > 0) moves |= bitboard >> 1;  // go left
    if (rank < 7 && file > 0) moves |= bitboard << 7; // to go up and to the left
    if (rank < 7 && file < 7) moves |= bitboard << 9; // to go up and to the right
    if (rank > 0 && file > 0) moves |= bitboard >> 9; //down and left
    if (rank > 0 && file < 7) moves |= bitboard >> 7; //down and right
    return moves; 
}


constexpr U64 compute_knight_moves(int square)
{
    U64 bitboard = 1ULL << square;
    U64 moves = 0ULL; 
    int rank = square/8;
    int file = square % 8;
    
    
    if (rank < 6 && file > 0) moves |= bitboard << 15;   // left 1 up 2
    if (rank < 6 && file < 7) moves |= bitboard << 17;   // right 1 up 2
    if (rank < 7 && file < 6) moves |= bitboard << 10;   // right 2 up 1
    if (rank > 0 && file < 6) moves |= bitboard >> 6;    // right 2 down 1
    if (rank > 1 && file < 7) moves |= bitboard >> 15;   // right 1 down 2
    if (rank > 1 && file > 0) moves |= bitboard >> 17; //left 1 down 2
    if (rank < 7 && file > 1) moves |= bitboard << 6; // up 1 left 2
    if (rank > 0 && file > 1) moves |= bitboard >> 10; //left 2 down 1
    return moves; 
    
}

constexpr array<U64, 64> KINGMOVES = [] 
{
    array<U64, 64> arr{};
    for (int sq = 0; sq < 64; sq++)
    {
        arr[sq] = compute_king_moves(sq);
    }
    return arr;
}();

constexpr array<U64, 64> KNIGHTMOVES = [] 
{
    array<U64, 64> arr{};
    for (int sq = 0; sq < 64; sq++)
    {
        arr[sq] = compute_knight_moves(sq);
    }
    return arr;
}();

inline U64 get_king_attacks(int square, U64 allies)
{
    return KINGMOVES[square] & ~ allies;
}

inline U64 get_knight_attacks(int square, U64 allies)
{
    return KNIGHTMOVES[square] & ~ allies;
}

inline U64 get_rook_attacks(int sq, U64 blockers, U64 allies)
{
    blockers &= rmask[sq];
    int index = transform(blockers, RMagic[sq], RBits[sq]);
    return RookAttackTable[sq][index] & ~allies;
}

inline U64 get_bishop_attacks(int sq, U64 blockers, U64 allies)
{
    blockers &= bmask[sq];
    int index = transform(blockers, BMagic[sq], BBits[sq]);
    return BishopAttackTable[sq][index] & ~allies;
}

inline U64 get_queen_attacks(int sq, U64 blockers, U64 allies)
{
    return get_rook_attacks(sq, blockers, allies) | get_bishop_attacks(sq, blockers, allies);
}