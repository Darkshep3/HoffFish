#pragma once
#include <cstdint>
#define get_LSB(b) (__builtin_ctzll(b))

using U64 = uint64_t;

extern const int RBits[64];
extern const int BBits[64];

extern const U64 RMagic[64];
extern const U64 BMagic[64];

extern U64 rmask[64];
extern U64 bmask[64];

extern U64 RookAttackTable[64][4096];
extern U64 BishopAttackTable[64][512];

void init_magic();
void init_rmask();
void init_bmask();
void init_magic_tables();

U64 compute_rmask(int sq);
U64 compute_bmask(int sq);
U64 compute_ratt(int sq, U64 block);
U64 compute_batt(int sq, U64 block);
U64 index_to_U64(int index, int bits, U64 mask);

inline int pop_1st_bit(U64 &bb)
{
    int index = get_LSB(bb);
    bb &= (bb - 1);
    return index;
}

inline int transform (U64 b, U64 magic, int bits)
{
    return (int) ((b*magic) >> (64-bits));
}
