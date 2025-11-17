#pragma once
#include <cstdint>
#include <string>
typedef uint64_t U64;

#define set_bit(b, i) ((b) |= (1ULL << i))
#define get_bit(b, i) ((b) & (1ULL << i))
#define clear_bit(b, i) ((b) &= ~(1ULL << i))
#define get_LSB(b) (__builtin_ctzll(b))

class Bitboard {
public:
    U64 wpawns, wknights, wbishops, wrooks, wqueens, wking;
    U64 bpawns, bknights, bbishops, brooks, bqueens, bking;

    Bitboard();
    Bitboard(const Bitboard& other);
    void initialize();
    void display(U64 bitboard);
    void displayBoard();
    void movePiece(U64 &bitboard, int from, int to);
<<<<<<< HEAD
    U64 getWhitePieces();
    U64 getBlackPieces();
=======
    void setBit(char piece, int index);
>>>>>>> cc4588e (gamestate)
};