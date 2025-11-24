#include <cstdint> 
#include <cstdlib> 

typedef uint64_t U64;
#define get_LSB(b) (__builtin_ctzll(b))

//ill research this later because apparently using rand() 32 random bits could be low quality and have higher chance of collisions
//but i think this approach should not have errors? 
//technically after we generate this, we can store it as a txt then not need to run this again to save time! 

U64 randomU64(){
    U64 u1, u2, u3, u4;
    u1 = (U64)(rand() & 0xFFFF);
    u2 = (U64)(rand() & 0xFFFF);
    u3 = (U64)(rand() & 0xFFFF);
    u4 = (U64)(rand() & 0xFFFF);
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

U64 randomU64sparse(){
    return randomU64() & randomU64() & randomU64();
}

int count_1s(U64 b){
    int r = 0;
    while (b != 0){
        b &= b-1;
        r++;
    }
    return r;     
}

int pop_1st_bit(U64 *bb){
    int index = get_LSB(*bb);
    *bb &= (*bb - 1);
    return index;
}

U64 index_to_U64 (int index, int bits, U64 mask){
    int i;
    U64 result = 0ULL;
    for (int j = 0; j < bits; j++){
        i = pop_1st_bit(&mask);
        if (index & (1 << j))
            result |= (1ULL << i);
    }
    return result;
}

U64 rmask (int sq){
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

U64 bmask (int sq){
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

U64 ratt (int sq, U64 block){
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

U64 batt (int sq, U64 block){
    U64 result = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;
    for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++){
        result |= (1ULL << (f + r*8));
        if (block & (1ULL << (f + r*8))) break;
    }
    for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++){
        result |= (1ULL << (f + r*8));
        if (block & (1ULL << (f + r*8))) break;
    }
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--){
        result |= (1ULL << (f + r*8));
        if (block & (1ULL << (f + r*8))) break;
    }
    for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--){
        result |= (1ULL << (f + r*8));
        if (block & (1ULL << (f + r*8))) break;
    }
    return result;
}

int transform (U64 b, U64 magic, int bits){
    return (int) ((b*magic) >> (64-bits));
}