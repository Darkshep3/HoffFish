#pragma once
#include <cstdint>
#include <string>
typedef uint64_t U64;

#define set_bit(b, i) ((b) |= (1ULL << i))
#define get_bit(b, i) ((b) & (1ULL << i))
#define clear_bit(b, i) ((b) &= ~(1ULL << i))
#define get_LSB(b) (__builtin_ctzll(b))
inline void clear_LSB(U64 &b) {
    b &= (b - 1);
}


class Bitboard {
public:
    U64 wpawns, wknights, wbishops, wrooks, wqueens, wking;
    U64 bpawns, bknights, bbishops, brooks, bqueens, bking;

    Bitboard();
    Bitboard(const Bitboard& other);
    void display(U64 bitboard);
    void initialize();
    void displayBoard();
    void movePiece(U64 &bitboard, int from, int to);
    void charToBit(char piece, int index);
    U64 getWhitePieces();
    U64 getBlackPieces();
    U64 getOccupied();
    U64 getEmpty();
};