#pragma once
#include <cstdint>
#include <array> 

using U64 = uint64_t;
using namespace std;

//precomputed King / Move attacks lookup tables 


constexpr U64 computeKingMoves(int square){
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


constexpr U64 computeKnightMoves(int square){
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

constexpr array<U64, 64> KINGMOVES = [] {
    array<U64, 64> arr{};
    for (int sq = 0; sq < 64; sq++){
        arr[sq] = computeKingMoves(sq);
    }
    return arr;
}();

constexpr array<U64, 64> KNIGHTMOVES = [] {
    array<U64, 64> arr{};
    for (int sq = 0; sq < 64; sq++){
        arr[sq] = computeKnightMoves(sq);
    }
    return arr;
}();

U64 getKingAttacks(int square, U64 allies){
    return KINGMOVES[square] & ~ allies;
}

U64 getKnightAttacks(int square, U64 allies){
    return KNIGHTMOVES[square] & ~ allies;
}