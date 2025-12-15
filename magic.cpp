#include "Magic.h"

const int RBits[64] = 
{
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12
};

const int BBits[64] = 
{
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6
};

const U64 RMagic[64] = 
{
0x80004000976080ULL, 0x1040400010002000ULL, 0x4880200210000980ULL, 0x5280080010000482ULL, 0x200040200081020ULL, 0x2100080100020400ULL, 0x4280008001000200ULL,
0x1000a4425820300ULL, 0x29002100800040ULL, 0x4503400040201004ULL, 0x209002001004018ULL, 0x1131000a10002100ULL, 0x9000800120500ULL, 0x10e001804820010ULL,
0x29000402000100ULL, 0x2002000d01c40292ULL, 0x80084000200c40ULL, 0x10004040002002ULL, 0x201030020004014ULL, 0x80012000a420020ULL, 0x129010008001204ULL,
0x6109010008040002ULL, 0x950010100020004ULL, 0x803a0000c50284ULL, 0x80004100210080ULL, 0x200240100140ULL, 0x20004040100800ULL, 0x4018090300201000ULL,
0x4802010a00102004ULL, 0x2001000900040002ULL, 0x4a02104001002a8ULL, 0x2188108200204401ULL, 0x40400020800080ULL, 0x880402000401004ULL, 0x10040800202000ULL,
0x604410a02001020ULL, 0x200200206a001410ULL, 0x86000400810080ULL, 0x428200040600080bULL, 0x2001000041000082ULL, 0x80002000484000ULL, 0x210002002c24000ULL,
0x401a200100410014ULL, 0x5021000a30009ULL, 0x218000509010010ULL, 0x4000400410080120ULL, 0x20801040010ULL, 0x29040040820011ULL, 0x4080400024800280ULL,
0x500200040100440ULL, 0x2880142001004100ULL, 0x412020400a001200ULL, 0x18c028004080080ULL, 0x884001020080401ULL, 0x210810420400ULL, 0x801048745040200ULL,
0x4401002040120082ULL, 0x408200210012ULL, 0x110008200441ULL, 0x2010002004100901ULL, 0x801000800040211ULL, 0x480d000400820801ULL, 0x820104201280084ULL,
0x1001040311802142ULL
};

const U64 BMagic[64] = 
{
0x1024b002420160ULL, 0x1008080140420021ULL, 0x2012080041080024ULL, 0xc282601408c0802ULL, 0x2004042000000002ULL, 0x12021004022080ULL, 0x880414820100000ULL,
0x4501002211044000ULL, 0x20402222121600ULL, 0x1081088a28022020ULL, 0x1004c2810851064ULL, 0x2040080841004918ULL, 0x1448020210201017ULL, 0x4808110108400025ULL,
0x10504404054004ULL, 0x800010422092400ULL, 0x40000870450250ULL, 0x402040408080518ULL, 0x1000980a404108ULL, 0x1020804110080ULL, 0x8200c02082005ULL,
0x40802009a0800ULL, 0x1000201012100ULL, 0x111080200820180ULL, 0x904122104101024ULL, 0x4008200405244084ULL, 0x44040002182400ULL, 0x4804080004021002ULL,
0x6401004024004040ULL, 0x404010001300a20ULL, 0x428020200a20100ULL, 0x300460100420200ULL, 0x404200c062000ULL, 0x22101400510141ULL, 0x104044400180031ULL,
0x2040040400280211ULL, 0x8020400401010ULL, 0x20100110401a0040ULL, 0x100101005a2080ULL, 0x1a008300042411ULL, 0x120a025004504000ULL, 0x4001084242101000ULL,
0xa020202010a4200ULL, 0x4000002018000100ULL, 0x80104000044ULL, 0x1004009806004043ULL, 0x100401080a000112ULL, 0x1041012101000608ULL, 0x40400c250100140ULL,
0x80a10460a100002ULL, 0x2210030401240002ULL, 0x6040aa108481b20ULL, 0x4009004050410002ULL, 0x8106003420200e0ULL, 0x1410500a08206000ULL, 0x92548802004000ULL,
0x1040041241028ULL, 0x120042025011ULL, 0x8060104054400ULL, 0x20004404020a0a01ULL, 0x40008010020214ULL, 0x4000050209802c1ULL, 0x208244210400ULL,
0x10140848044010ULL
};

U64 rmask[64];

U64 bmask[64];

U64 RookAttackTable[64][4096];
U64 BishopAttackTable[64][512];

U64 index_to_U64 (int index, int bits, U64 mask)
{
    int i;
    U64 result = 0ULL;
    for (int j = 0; j < bits; j++){
        i = pop_1st_bit(mask);
        if (index & (1 << j))
            result |= (1ULL << i);
    }
    return result;
}

//computing all the rook attack moves
U64 compute_rmask (int sq)
{
    U64 result = 0ULL;
    int rank = sq/8;
    int file = sq % 8;
    for (int r = rank + 1; r <= 6; r++)
        result |= (1ULL << (file + r*8));
    for (int r = rank - 1; r >= 1; r--)
        result |= (1ULL << (file + r*8));
    for (int f = file + 1; f <= 6; f++)
        result |= (1ULL << (rank*8 + f));
    for (int f = file - 1; f >= 1; f--)
        result |= (1ULL << (rank*8 + f));
    return result;
}

//computing all the bishop attack moves

U64 compute_bmask (int sq)
{
    U64 result = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;
    for (int r = rank + 1, f = file + 1; r <= 6 && f <= 6; r++, f++)
        result |= (1ULL << (f + r*8));
    for (int r = rank - 1, f = file + 1; r >= 1 && f <= 6; r--, f++)
        result |= (1ULL << (f + r*8));
    for (int r = rank - 1, f = file - 1; r >= 1 && f >= 1; r--, f--)
        result |= (1ULL << (f + r*8));
    for (int r = rank + 1, f = file - 1; r <= 6 && f >= 1; r++, f--)
        result |= (1ULL << (f + r*8));
    return result;
}

//computing all the rook attack moves with blockers

U64 compute_ratt (int sq, U64 block)
{
    U64 result = 0ULL;
    int rank = sq/8;
    int file = sq % 8;
    for (int r = rank + 1; r <= 7; r++){
        result |= (1ULL << (file + r*8));
        if (block & (1ULL << (file + r*8))) break;
    }
    for (int r = rank - 1; r >= 0; r--){
        result |= (1ULL << (file + r*8));
        if (block & (1ULL << (file + r*8))) break;
    }
    for (int f = file + 1; f <= 7; f++){
        result |= (1ULL << (rank*8 + f)); 
        if (block & (1ULL << (f + rank*8))) break;
    }
    for (int f = file - 1; f >= 0; f--){
        result |= (1ULL << (rank*8 + f));
        if (block & (1ULL << (f + rank*8))) break;
    }
    return result;
}

//computing all the bishop attack moves with blockers

U64 compute_batt (int sq, U64 block)
{
    U64 result = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;
    for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++)
    {
        result |= (1ULL << (f + r*8));
        if (block & (1ULL << (f + r*8))) break;
    }
    for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++)
    {
        result |= (1ULL << (f + r*8));
        if (block & (1ULL << (f + r*8))) break;
    }
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
    {
        result |= (1ULL << (f + r*8));
        if (block & (1ULL << (f + r*8))) break;
    }
    for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--)
    {
        result |= (1ULL << (f + r*8));
        if (block & (1ULL << (f + r*8))) break;
    }
    return result;
}

void init_rmask()
{
    for (int sq = 0; sq < 64; sq++)
    {
        rmask[sq] = compute_rmask(sq);
    }
}

void init_bmask()
{
    for (int sq = 0; sq < 64; sq++)
    {
        bmask[sq] = compute_bmask(sq);
    }
}

void init_magic_tables()
{
    //initializing rook attack tables
    for (int sq = 0; sq < 64; sq++)
    {
        U64 mask = rmask[sq];
        int bits = RBits[sq];
        int max = 1 << bits;
        for (int i = 0; i < max; i++){
            U64 blockers = index_to_U64(i, bits, mask);
            int index = transform(blockers, RMagic[sq], bits);
            RookAttackTable[sq][index] = compute_ratt(sq, blockers);
        }
    }
    //initializing bishop attack tables
    for (int sq = 0; sq < 64; sq++)
    {
        U64 mask = bmask[sq];
        int bits = BBits[sq];
        int max = 1 << bits;

        for (int i = 0; i < max; i++){
            U64 blockers = index_to_U64(i, bits, mask);
            int index = transform(blockers, BMagic[sq], bits);
            BishopAttackTable[sq][index] = compute_batt(sq, blockers);
        }
    }
}

void init_magic()
{
    init_rmask();
    init_bmask();
    init_magic_tables();
}