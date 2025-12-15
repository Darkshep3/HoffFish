#pragma once
#include <cstdint>
#include <string>
typedef uint64_t U64;

#define set_bit(b, i) ((b) |= (1ULL << i))
#define get_bit(b, i) ((b) & (1ULL << i))
#define clear_bit(b, i) ((b) &= ~(1ULL << i))
#define get_LSB(b) (__builtin_ctzll(b))
inline void clear_LSB(U64 &b) 
{
    b &= (b - 1);
}
constexpr U64 FILE_A = 0x0101010101010101ULL;
constexpr U64 FILE_B = 0x0202020202020202ULL;
constexpr U64 FILE_C = 0x0404040404040404ULL;
constexpr U64 FILE_D = 0x0808080808080808ULL;
constexpr U64 FILE_E = 0x1010101010101010ULL;
constexpr U64 FILE_F = 0x2020202020202020ULL;
constexpr U64 FILE_G = 0x4040404040404040ULL;
constexpr U64 FILE_H = 0x8080808080808080ULL;
constexpr U64 FILES[8] = 
{
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, 
};

class Bitboard 
{
public:
    U64 wpawns, wknights, wbishops, wrooks, wqueens, wking;
    U64 bpawns, bknights, bbishops, brooks, bqueens, bking;
    U64 pieceBb[12];
    char pieceChars[12] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};

    Bitboard();
    Bitboard(const Bitboard& other);
    void display(U64 bitboard);
    void initialize();
    void update_piece_bb(); 
    void display_board();
    void clear_square(int index);
    void move_piece(int from, int to, char piece);
    void place_piece(char piece, int index);
    U64 get_white_pieces();
    U64 get_black_pieces();
    // void setBit(char piece, int index);
    U64 get_occupied();
    U64 get_empty();
    char get_piece_at(int index);
};