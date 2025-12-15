// #include <cstdint> 
// #include <cstdlib> 
// #include <iostream>
// #include "Magic.h"
#include "MagicGenerator.h"

//Credit to Niklas Fiekas for this Magic Bitboard generation! 
//ill research this later because apparently using rand() 32 random bits could be low quality and have higher chance of collisions
//but i think this approach should not have errors? 
//technically after we generate this, we can store it as a txt then not need to run this again to save time! 

U64 randomU64()
{
    U64 u1, u2, u3, u4;
    u1 = (U64)(rand() & 0xFFFF);
    u2 = (U64)(rand() & 0xFFFF);
    u3 = (U64)(rand() & 0xFFFF);
    u4 = (U64)(rand() & 0xFFFF);
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

U64 randomU64_fewbits()
{
    return randomU64() & randomU64() & randomU64();
}

int count_1s(U64 b)
{
    int r = 0;
    while (b != 0){
        b &= b-1;
        r++;
    }
    return r;     
}

U64 find_magic (int sq, int m, int bishop)
{
    U64 mask, b[4096], a[4096], used[4096], magic;
    int i, j, k, n, fail;
    
    mask = bishop ? compute_bmask(sq) : compute_rmask(sq);
    n = count_1s(mask);


    for (i = 0; i < (1<< n); i++)
    {
        b[i] = index_to_U64(i, n, mask); //all possible occupancy under the mask
        a[i] = bishop ? compute_batt(sq, b[i]) : compute_ratt(sq, b[i]); //all possible attacks under the mask
    }
    for (k = 0; k < 10000000; k++)
    {
        magic = randomU64_fewbits();
        if (count_1s((mask * magic) & 0xFF00000000000000ULL) < 6) continue;
        //Filters out the ones with way too few 1 bits (more likey to produce collisions)
        for(i = 0; i < 4096; i++) used[i] = 0ULL;
        for (i = 0, fail = 0; !fail && i < (1 << n); i++)
        {
            j = transform(b[i], magic, m);
            if(used[j] == 0ULL) used[j] = a[i];
            else if(used[j] != a[i]) fail = 1;
        }
        if(!fail) return magic;
    }
    printf("***Failed***\n");
        return 0ULL;
}

int magic_generator() 
{
    int square;
    printf("const U64 RMagic[64] = {\n");
    for(square = 0; square < 64; square++)
    {
    if (square % 7 == 6)
        printf("0x%llxULL,\n", find_magic(square, RBits[square], 0));
    else 
        printf("0x%llxULL, ", find_magic(square, RBits[square], 0));
    }
    printf("};\n\n");

    printf("const U64 BMagic[64] = {\n");
    for(square = 0; square < 64; square++)
    {
    if (square % 7 == 6)
        printf("0x%llxULL,\n", find_magic(square, BBits[square], 1));
    else 
        printf("0x%llxULL, ", find_magic(square, BBits[square], 1));
    }
    printf("};\n\n");
    return 0;
}