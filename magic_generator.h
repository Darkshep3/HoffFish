#include <cstdint> 
#include <cstdlib> 
#include <iostream>
#include "magic.h"

typedef uint64_t U64;

U64 randomU64();
U64 randomU64_fewbits();
int count_1s(U64 b);
U64 find_magic (int sq, int m, int bishop);
int magic_generator();